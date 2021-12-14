# FUNCTIONS


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [Using SUM, Count, MAX, DISTINCT and ORDER BY](#using-sum-count-max-distinct-and-order-by)
- [Using GROUP BY and HAVING](#using-group-by-and-having)
- [CONCAT, SUBSTRING, LOWER, POSITION](#concat-substring-lower-position)
- [ROUND, COALESCE, CASE](#round-coalesce-case)
- [CURRENT_TIMESTAMP, CURRENT_DATE, DATE_FORMAT, EXTRACT](#current_timestamp-current_date-date_format-extract)
- [REPLACE, INSTR, IFNULL, NVL, NULLIF](#replace-instr-ifnull-nvl-nullif)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Using SUM, Count, MAX, DISTINCT and ORDER BY](#using-sum-count-max-distinct-and-order-by)
- [Using GROUP BY and HAVING](#using-group-by-and-having)
- [CONCAT, SUBSTRING, LOWER, POSITION](#concat-substring-lower-position)
  - [Concatenate strings](#concatenate-strings)
  - [Extracting substrings](#extracting-substrings)
  - [Lower case](#lower-case)
  - [Finding a substring in a string](#finding-a-substring-in-a-string)
- [ROUND, COALESCE, CASE](#round-coalesce-case)
  - [Formating numbers to two decimal spaces](#formating-numbers-to-two-decimal-spaces)
  - [COALESCE, Replace a NULL with specific value](#coalesce-replace-a-null-with-specific-value)
  - [Conditional values and If](#conditional-values-and-if)
- [CURRENT_TIMESTAMP, CURRENT_DATE, DATE_FORMAT, EXTRACT](#current_timestamp-current_date-date_format-extract)
  - [Get a date and time right now](#get-a-date-and-time-right-now)
  - [Format dates](#format-dates)
  - [EXTRACT, HOUR and others](#extract-hour-and-others)
- [REPLACE, INSTR, IFNULL, NVL, NULLIF](#replace-instr-ifnull-nvl-nullif)
  - [REPLACE](#replace)
  - [INSTR](#instr)
  - [IFNULL and NVL](#ifnull-and-nvl)
  - [NULLIF](#nullif)

<!-- /code_chunk_output -->

### Using SUM, Count, MAX, DISTINCT and ORDER BY

```sql
--The total population and GDP of Europe.
SELECT SUM(population), SUM(gdp)
  FROM bbc
  WHERE region = 'Europe'

--What are the regions?
SELECT DISTINCT region FROM bbc /* 写作 DISTINCT(region) 也可 */

--Show the name and population for each country with a population
--  of more than 100000000. Show countries in descending order of population.
SELECT name, population
  FROM bbc
  WHERE population > 100000000
  ORDER BY population DESC
```

### Using GROUP BY and HAVING

`GROUP BY <键值>` 是把所有相同键值的行放到一行，然后用聚合函数可以把其他字段处理一下。

`HAVING` 是与 `GROUP BY` 联用的，在 `GROUP BY` 前使用 `WHERE` 是针对某一行进行筛选，如何对这一个聚类块进行筛选呢？用 `HAVING` 。

```sql
--For each continent show the number of countries
SELECT continent, COUNT(name)
  FROM world
 GROUP BY continent

--For each continent show the total population:
SELECT continent, SUM(population)
  FROM world
 GROUP BY continent

--WHERE and GROUP BY. The WHERE filter takes place before the aggregating function.
--  For each relevant continent show the number of countries that has
--  a population of at least 200000000.
SELECT continent, COUNT(name)
  FROM world
 WHERE population>200000000
 GROUP BY continent  /* GROUP BY 要放在最后 */

--GROUP BY and HAVING. The HAVING clause is tested after the GROUP BY.
--  You can test the aggregated values with a HAVING clause. Show the
--  total population of those continents with a total population
--  of at least half a billion.
SELECT continent, SUM(population)
  FROM world
 GROUP BY continent
HAVING SUM(population)>500000000
```

### CONCAT, SUBSTRING, LOWER, POSITION

#### Concatenate strings

```sql
--Concatenation means "stick strings together". In this example we concatenate three strings,
--  name and region are string attributes of the table, ' is in ' is a string literal.
SELECT CONCAT(name, ' is in ', region)
  FROM bbc
  WHERE name LIKE 'D%'
```

#### Extracting substrings

We from position 1 (the beginning) we take two characters. 4.

```
'Afghanistan' -> 'Af'
'China'       -> 'Ch'
'Sri Lanka'   -> 'Sr'
```

The SQL standard insists on a horrible syntax:

```sql
SUBSTRING(name FROM 1 FOR 2) 
```

```sql
SELECT name, SUBSTRING(name FROM 1 FOR 2)
 FROM bbc
```

#### Lower case

```sql
--UPPER CASE is similar.
SELECT LOWER(name) FROM bbc
 WHERE UPPER(region)='SOUTH AMERICA'
```

#### Finding a substring in a string

```sql
SELECT name,
       POSITION(' ' IN name),
       SUBSTRING(name FROM 1 FOR POSITION(' ' IN name))
  FROM bbc
  WHERE name LIKE '% %'  /* 只要含有空格的字段 */
```

结果：
```
United Kingdom	7	United
Israel and Palestinian territories	7	Israel
Saudi Arabia	6	Saudi
United Arab Emirates	7	United
United States of America	7	United
```

### ROUND, COALESCE, CASE

#### Formating numbers to two decimal spaces

```sql
SELECT name,
   population,
   ROUND(population/1000000,2),
   ROUND(population,-6)
  FROM bbc
  WHERE region='North America'
```

结果：
```
name	population	ROUND(populat..	ROUND(populat..
Canada	32000000	32	32000000
Mexico	106400000	106.4	106000000
United States of America	295000000	295	295000000
```

#### COALESCE, Replace a NULL with specific value

COALESCE takes any number of arguments and returns the first value that is not null.

```sql
COALESCE(x,y,z) = x if x is not NULL
COALESCE(x,y,z) = y if x is NULL and y is not NULL
COALESCE(x,y,z) = z if x and y are NULL but z is not NULL
COALESCE(x,y,z) = NULL if x and y and z are all NULL
```

就是依次检查表达式，哪个不空返回哪个，全为 `NULL` 就返回 `NULL` 。

```sql
--COALESCE can be useful when you want to replace a NULL value with some other value.
--  In this example you show the name of the party for each MSP that has a party.
--  For the MSP with no party (such as Canavan, Dennis) you show the string None.
SELECT name, party
      ,COALESCE(party,'None') AS aff
  FROM msp WHERE name LIKE 'C%'
```

结果：
```
name									party			aff
Campbell MSP, Colin						SNP				SNP
Canavan MSP, Dennis										None
Chisholm MSP, Malcolm					Lab				Lab
Craigie MSP, Cathie						Lab				Lab
Crawford JP MSP, Bruce					SNP				SNP
Cunningham MSP, Roseanna				SNP				SNP
Curran MSP, Ms Margaret					Lab				Lab
```

```sql
SELECT code, name,
    COALESCE(leader, 'NO LEADER RECORDED!')
  FROM party
```

结果：
```
code	name	COALESCE(lead..
Com	Communist	NO LEADER RECORDED!
Con	Conservative	McLetchie MSP, David
Green	Green	NO LEADER RECORDED!
Lab	Labour	Dewar MSP, Rt Hon Donald
LD	Liberal Democrat	Wallace QC MSP, Mr Jim
NLP	Natural Law Party	NO LEADER RECORDED!
SNP	Scottish National Party	Salmond MSP, Mr Alex
SSP	Scottish Socialist Party	NO LEADER RECORDED!
SWP	Socialist Workers Party	NO LEADER RECORDED!
Functions
```

#### Conditional values and If

`CASE` allows you to return different values under different conditions.

If there no conditions match (and there is not `ELSE`) then `NULL` is returned.

```sql
CASE WHEN condition1 THEN value1 
      WHEN condition2 THEN value2  
      ELSE def_value 
END
```

示例：
```sql
SELECT name, population
      ,CASE WHEN population<1000000 
            THEN 'small'
            WHEN population<10000000 
            THEN 'medium'
            ELSE 'large'
       END
  FROM bbc

SELECT
   title, 
   yr,
   CASE WHEN yr>2000 THEN 'New'
        ELSE 'Old'
   END
   FROM movie
```

上述情况使用 `IF` 关键字也不错：

```sql
SELECT
   title, 
   yr,
   IF(yr>2000,'New', 'Old')
   END
   FROM movie
```

### CURRENT_TIMESTAMP, CURRENT_DATE, DATE_FORMAT, EXTRACT

#### Get a date and time right now

```sql
--CURRENT_TIMESTAMP returns the current date and time.
--  CURRENT_TIMESTAMP -> '2006-12-31 18:03:44'  
SELECT CURRENT_TIMESTAMP, whn
  FROM eclipse
/*
CURRENT_TIMES..	whn
Tue, 14 Dec 2021 02:23:48 GMT	Tue, 04 Jan 2011 00:00:00 GMT
Tue, 14 Dec 2021 02:23:48 GMT	Wed, 01 Jun 2011 00:00:00 GMT
Tue, 14 Dec 2021 02:23:48 GMT	Fri, 01 Jul 2011 00:00:00 GMT
Tue, 14 Dec 2021 02:23:48 GMT	Fri, 25 Nov 2011 00:00:00 GMT
*/

--CURRENT_DATE returns today's date.
--  CURRENT_DATE -> '2006-12-31'
SELECT CAST(CURRENT_DATE AS DATE),wk
  FROM totp
/*
CAST(CURRENT_..	wk
Tue, 14 Dec 2021 00:00:00 GMT	Wed, 27 Nov 1974 00:00:00 GMT
Tue, 14 Dec 2021 00:00:00 GMT	Wed, 27 Nov 1974 00:00:00 GMT
Tue, 14 Dec 2021 00:00:00 GMT	Wed, 27 Nov 1974 00:00:00 GMT
*/
```

#### Format dates

Many engines support the SQL standard - see Mimer （一种 SQL 引擎） for details.

```sql
SELECT DATE_FORMAT(wk,'%d/%m/%Y'), song
FROM totp
WHERE singer='Tom Jones'
```

#### EXTRACT, HOUR and others

EXTRACT allows you to retrieve components of a date.

You can extract also `YEAR`, `MONTH`, `DAY`, `HOUR`, `MINUTE`, `SECOND`.

```
EXTRACT(YEAR FROM d)    EXTRACT(MONTH FROM d)
EXTRACT(DAY FROM d)     EXTRACT(HOUR FROM d)
EXTRACT(MINUTE FROM d)  EXTRACT(SECOND FROM d)
```

下面的例子中，在我看来和直接用 `YEAR(td)` 与 `HOUR(td)` 差不多。

```sql
SELECT whn
      ,EXTRACT(YEAR FROM td)  AS yr
      ,EXTRACT(HOUR FROM td) AS hr
  FROM eclipse
```

### REPLACE, INSTR, IFNULL, NVL, NULLIF

#### REPLACE

```sql
REPLACE('vessel','e','a') -> 'vassal'

--替换全部值
SELECT name,
       REPLACE(name, 'a','')
  FROM bbc
```

#### INSTR

如果没有返回 `0` 。

```sql
--In this example you get the position of string 'an' within a country's name.
SELECT name,
       INSTR(name, 'an')
  FROM bbc
/*
name	INSTR(name, '..
Angola	1
Benin	0
Botswana	6
Burkina Faso	0
Burundi	0
Cameroon	0
*/
```

#### IFNULL and NVL

二者一样的。

```
IFNULL(x,y) = x if x is not NULL
IFNULL(x,y) = y if x is NULL

NVL(x,y) = x if x is not NULL
NVL(x,y) = y if x is NULL
```

例子如下：

```sql
SELECT name, party
      ,IFNULL(party,'None') AS aff
  FROM msp WHERE name LIKE 'C%'
/*
name	party	aff
Campbell MSP, Colin	SNP	SNP
Canavan MSP, Dennis		None
Chisholm MSP, Malcolm	Lab	Lab
Craigie MSP, Cathie	Lab	Lab
Crawford JP MSP, Bruce	SNP	SNP
*/
```

#### NULLIF

`NULLIF` returns `NULL` if the two arguments are equal; otherwise `NULLIF` returns the first argument.

```
NULLIF(x,y) = NULL if x=y
NULLIF(x,y) = x if x != y
```

如果 `x` 值是 `y` ，那么就返回空值 `NULL` 。如果是空值也返回空值。

```sql
SELECT name, party
      ,NULLIF(party,'Lab') AS aff
  FROM msp WHERE name LIKE 'C%'
/*
name	party	aff
Campbell MSP, Colin	SNP	SNP
Canavan MSP, Dennis		
Chisholm MSP, Malcolm	Lab	
Craigie MSP, Cathie	Lab	
Crawford JP MSP, Bruce	SNP	SNP
*/
```
