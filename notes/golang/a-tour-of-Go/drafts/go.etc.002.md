# database.sql

看 [TDEngine](https://docs.taosdata.com/develop/connect/) 时产生的疑惑。
- https://github.com/taosdata/driver-go

```go
package main

import (
    "database/sql"
    "fmt"
    "log"

    _ "github.com/taosdata/driver-go/v3/taosSql"
)

func main() {
    var taosDSN = "root:taosdata@tcp(localhost:6030)/"
    taos, err := sql.Open("taosSql", taosDSN)
    if err != nil {
        log.Fatalln("failed to connect TDengine, err:", err)
        return
    }
    fmt.Println("Connected")
    defer taos.Close()
}

// use
// var taosDSN = "root:taosdata@tcp(localhost:6030)/dbName"
// if you want to connect a specified database named "dbName".
```

这里为什么用 [blank import](../../../../books/go/drafts/go.10.packages-and-the-go-tool.md) ？其实我认为最根本的原因是，为了隐式调用 `taosSql` 中的 `init` 函数，完成对于 sql 的注册。

```go
// https://github.com/taosdata/driver-go/blob/3.0/taosRestful/driver.go#L26
func init() {
	sql.Register("taosRestful", &TDengineDriver{})
}
```

这就要求 `TDengineDriver` 必须实现 `driver.Driver` 。

```go
// https://github.com/golang/go/blob/2ebe77a2fda1ee9ff6fd9a3e08933ad1ebaea039/src/database/sql/sql.go#L41
// Register makes a database driver available by the provided name.
// If Register is called twice with the same name or if driver is nil,
// it panics.
func Register(name string, driver driver.Driver) {
	driversMu.Lock()
	defer driversMu.Unlock()
	if driver == nil {
		panic("sql: Register driver is nil")
	}
	if _, dup := drivers[name]; dup {
		panic("sql: Register called twice for driver " + name)
	}
	drivers[name] = driver
}

// https://github.com/golang/go/blob/2ebe77a2fda1ee9ff6fd9a3e08933ad1ebaea039/src/database/sql/sql.go#L762
// Open opens a database specified by its database driver name and a
// driver-specific data source name, usually consisting of at least a
// database name and connection information.
//
// Most users will open a database via a driver-specific connection
// helper function that returns a *DB. No database drivers are included
// in the Go standard library. See https://golang.org/s/sqldrivers for
// a list of third-party drivers.
//
// Open may just validate its arguments without creating a connection
// to the database. To verify that the data source name is valid, call
// Ping.
//
// The returned DB is safe for concurrent use by multiple goroutines
// and maintains its own pool of idle connections. Thus, the Open
// function should be called just once. It is rarely necessary to
// close a DB.
func Open(driverName, dataSourceName string) (*DB, error) {
	driversMu.RLock()
	driveri, ok := drivers[driverName]
	driversMu.RUnlock()
	if !ok {
		return nil, fmt.Errorf("sql: unknown driver %q (forgotten import?)", driverName)
	}

	if driverCtx, ok := driveri.(driver.DriverContext); ok {
		connector, err := driverCtx.OpenConnector(dataSourceName)
		if err != nil {
			return nil, err
		}
		return OpenDB(connector), nil
	}

	return OpenDB(dsnConnector{dsn: dataSourceName, driver: driveri}), nil
}
```

### database.sql 设计意图

应用层参考文章 [Go database/sql 教程](https://developer.aliyun.com/article/178898) 。

```go
import (
    "database/sql"
    _ "github.com/jackx/pgx/stdlib"
)

func main() {
    db, err := sql.Open("pgx","postgres://localhost:5432/postgres")
    if err != nil {
        log.Fatal(err)
    }
    defer db.Close()

    // 正如 Open 的注释
    // 执行sql.Open()并未实际建立起到数据库的连接，也不会验证驱动参数。
    // 第一个实际的连接会惰性求值，延迟到第一次需要时建立。用户应该通过db.Ping()来检查数据库是否实际可用。
    if err = db.Ping(); err != nil {
        // do something about db error
    }
    
}
```

- `sql.DB` 对象是为了长连接而设计的，不要频繁 `Open()` 和 `Close()` 数据库。
- 而应该为每个待访问的数据库创建一个 `sql.DB` 实例，并在用完前一直保留它。需要时可将其作为参数传递，或注册为全局对象。

### Query 与 rows

```go
func example() {
    var sum, n int32

    // invoke query
    rows, err := db.Query("SELECT generate_series(1,$1)", 10)
    // handle query error
    if err != nil {
        fmt.Println(err)
    }
    // defer close result set
    defer rows.Close()

    // Iter results
    for rows.Next() {
        if err = rows.Scan(&n); err != nil {
            fmt.Println(err)    // Handle scan error
        }
        sum += n                // Use result
    }

    // check iteration error
    if rows.Err() != nil {
        fmt.Println(err)
    }

    fmt.Println(sum)
}
```

这里 `db *DB` 的 `Query` 返回了一个 `*Rows` 。而这个 `Rows` 其中包裹了 `driver.Rows` 。注意，并不需要 `driver.Rows` 继承 `sql.Rows` 。

```go
// https://github.com/golang/go/blob/2ebe77a2fda1ee9ff6fd9a3e08933ad1ebaea039/src/database/sql/sql.go

// Query executes a query that returns rows, typically a SELECT.
// The args are for any placeholder parameters in the query.
//
// Query uses context.Background internally; to specify the context, use
// QueryContext.
func (db *DB) Query(query string, args ...interface{}) (*Rows, error) {
	return db.QueryContext(context.Background(), query, args...)
}

// QueryContext executes a query that returns rows, typically a SELECT.
// The args are for any placeholder parameters in the query.
func (c *Conn) QueryContext(ctx context.Context, query string, args ...interface{}) (*Rows, error) {
	dc, release, err := c.grabConn(ctx)
	if err != nil {
		return nil, err
	}
	return c.db.queryDC(ctx, nil, dc, release, query, args)
}

// queryDC executes a query on the given connection.
// The connection gets released by the releaseConn function.
// The ctx context is from a query method and the txctx context is from an
// optional transaction context.
func (db *DB) queryDC(ctx, txctx context.Context, dc *driverConn, releaseConn func(error), query string, args []interface{}) (*Rows, error) {
	queryerCtx, ok := dc.ci.(driver.QueryerContext)
	var queryer driver.Queryer
	if !ok {
		queryer, ok = dc.ci.(driver.Queryer)
	}
	if ok {
		var nvdargs []driver.NamedValue
		var rowsi driver.Rows
		var err error
		withLock(dc, func() {
			nvdargs, err = driverArgsConnLocked(dc.ci, nil, args)
			if err != nil {
				return
			}
			rowsi, err = ctxDriverQuery(ctx, queryerCtx, queryer, query, nvdargs)
		})
		if err != driver.ErrSkip {
			if err != nil {
				releaseConn(err)
				return nil, err
			}
			// Note: ownership of dc passes to the *Rows, to be freed
			// with releaseConn.
			rows := &Rows{
				dc:          dc,
				releaseConn: releaseConn,
				rowsi:       rowsi,
			}
			rows.initContextClose(ctx, txctx)
			return rows, nil
		}
	}

	var si driver.Stmt
	var err error
	withLock(dc, func() {
		si, err = ctxDriverPrepare(ctx, dc.ci, query)
	})
	if err != nil {
		releaseConn(err)
		return nil, err
	}

	ds := &driverStmt{Locker: dc, si: si}
	rowsi, err := rowsiFromStatement(ctx, dc.ci, ds, args...)
	if err != nil {
		ds.Close()
		releaseConn(err)
		return nil, err
	}

	// Note: ownership of ci passes to the *Rows, to be freed
	// with releaseConn.
	rows := &Rows{
		dc:          dc,
		releaseConn: releaseConn,
		rowsi:       rowsi,
		closeStmt:   ds,
	}
	rows.initContextClose(ctx, txctx)
	return rows, nil
}
```
