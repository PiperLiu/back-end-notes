# ALTER, UNION, Full Text Search, Equi Join, Column Name with Spaces


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [ALTER](#alter)
- [UNION](#union)
- [Full Text Search](#full-text-search)
- [Equi Join](#equi-join)
- [Column Name with Spaces](#column-name-with-spaces)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [ALTER](#alter)
- [UNION](#union)
- [Full Text Search](#full-text-search)
  - [注意：版本问题](#注意版本问题)
  - [创建全文索引 FULLTEXT](#创建全文索引-fulltext)
  - [自然语言搜索](#自然语言搜索)
  - [BOOLEAN MODE](#boolean-mode)
- [Equi Join](#equi-join)
- [Column Name with Spaces](#column-name-with-spaces)

<!-- /code_chunk_output -->

### ALTER

The ALTER statement can be used to add a column. In this example the column season indicates if the games are winter or summer games.

```sql
SELECT * FROM games;
ALTER TABLE games ADD season VARCHAR(6);
UPDATE games SET season = 'summer'
  WHERE yr = 2004;
UPDATE games SET season = 'winter'
  WHERE yr = 2006;
SELECT * FROM games;
```

### UNION

List a number of SELECT statements separated by the UNION key word. Be sure that you have the same number of columns in each of the SELECT statements.

```sql
SELECT name FROM bbc WHERE name LIKE 'Z%'
UNION
SELECT name FROM actor WHERE name LIKE 'Z%'
```

### Full Text Search

参考[Mysql使用全文索引(FullText index) - 程序猿小哈的文章 - 知乎](https://zhuanlan.zhihu.com/p/88275060)和[MySQL使用全文索引(fulltext index)](https://blog.csdn.net/u011734144/article/details/52817766/)：

#### 注意：版本问题

在`MySQL 5.6`版本以前，只有`MyISAM`存储引擎支持全文引。在`5.6`版本中，`InnoDB`加入了对全文索引的支持，但是不支持中文全文索引。在`5.7.6`版本，`MySQL`内置了`ngram`全文解析器，用来支持亚洲语种的分词。

#### 创建全文索引 FULLTEXT

```sql
CREATE TABLE articles (
    id INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY,
    title VARCHAR (200),
    body TEXT,
    FULLTEXT (title, body) WITH PARSER ngram
) ENGINE = INNODB DEFAULT CHARSET=utf8mb4 COMMENT='文章表';
```

如上，创建了一个给`title`和`body`字段添加全文缩影的表。

另一种方法，用 `ALERT` 给字段添加全文索引：
```sql
ALTER TABLE articles
  ADD FULLTEXT INDEX title_body_index (title, body)
  WITH PARSER ngram;
```

也可以直接通过 `create index` 的方式：
```sql
CREATE FULLTEXT INDEX ft_email_name ON `student` (`name`)
// 也可以在创建索引的时候指定索引的长度
CREATE FULLTEXT INDEX ft_email_name ON `student` (`name`(20))
```

#### 自然语言搜索

普通的判断是否包含：

```sql
mysql> SELECT * FROM articles WHERE MATCH (title,body) AGAINST ('精神' IN NATURAL LANGUAGE MODE);
+----+-----------------+-------------------------+
| id | title           | body                    |
+----+-----------------+-------------------------+
|  1 | 弘扬正能量      | 贯彻党的18大精神        |
+----+-----------------+-------------------------+
1 row in set (0.00 sec)

mysql> SELECT * FROM articles WHERE MATCH (title,body) AGAINST ('精神');
+----+-----------------+-------------------------+
| id | title           | body                    |
+----+-----------------+-------------------------+
|  1 | 弘扬正能量      | 贯彻党的18大精神        |
+----+-----------------+-------------------------+
1 row in set (0.00 sec)
```

可以看到，搜索结果命中了一条，且在不指定搜索模式的情况下，默认模式为自然语言搜索。

#### BOOLEAN MODE

这个模式和 `lucene` 中的 `BooleanQuery` 很像，可以通过一些操作符，来指定搜索词在结果中的包含情况。比如 `+` 表示必须包含， `-` 表示必须不包含，默认为误操作符，代表可以出现可以不出现，但是出现时在查询结果集中的排名较高一些，也就是该结果和搜索词的相关性高一些。

具体包含的所有操作符可以通过 MySQL 查询来查看：

```sql
mysql> show variables like '%ft_boolean_syntax%'

+-------------------+----------------+
| Variable_name     | Value          |
+-------------------+----------------+
| ft_boolean_syntax | + -><()~*:""&| |
+-------------------+----------------+
mysql> SELECT * FROM articles WHERE MATCH (title,body) AGAINST ('+精神' IN BOOLEAN MODE);
+----+-----------------+-------------------------+
| id | title           | body                    |
+----+-----------------+-------------------------+
|  1 | 弘扬正能量      | 贯彻党的18大精神        |
+----+-----------------+-------------------------+
1 row in set (0.00 sec)

mysql> SELECT * FROM articles WHERE MATCH (title,body) AGAINST ('+精神 -贯彻' IN BOOLEAN MODE);
Empty set (0.01 sec)
```

当搜索必须命中精神时，命中了一条数据，当在加上不能包含贯彻的时候，无命中结果。

### Equi Join

其实是个很基础的概念。

I am trying to join two tables with the same name. I am required to use an inner join Also I need to show the managers name. Question is: Join the employee table with the employee table and department table. Show the employee id, name, dept code, manager id, manager name, managers department code, and name for that (mgr's) department.

We have a self join, each copy of the table is given an "alias" - here we use w for the worker and b for the boss. We can treat these as different tables. You get an inner join by default - this means that Robin (who has no boss) does not show up in the results.

```sql
CREATE TABLE employee(
  employee_id INTEGER PRIMARY KEY,
  first_name VARCHAR(10),
  dept_code VARCHAR(10),
  manager_id INTEGER REFERENCES employee);
INSERT INTO employee VALUES (1,'Robin','Eng',NULL);
INSERT INTO employee VALUES (2,'Jon','SoC',1);
INSERT INTO employee VALUES (3,'Andrew','SoC',2);
INSERT INTO employee VALUES (4,'Alison','SoC',2);
SELECT w.first_name as worker, b.first_name as boss
 FROM employee w, employee b
WHERE w.manager_id = b.employee_id
```

结果：

|worker|boss|
|---|---|
|Jon|Robin|
|Andrew|Jon|
|Alison|Jon|

### Column Name with Spaces

You can access columns where the name contains a space.

```sql
CREATE TABLE SpaceMonster(`Account Balance` INT);
INSERT INTO SpaceMonster VALUES (42);
SELECT `Account Balance` FROM SpaceMonster
```
