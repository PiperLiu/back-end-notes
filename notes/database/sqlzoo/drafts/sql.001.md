# SELECT basics


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [基础的查询与运算](#基础的查询与运算)
- [Umlaut 非ASC码字符 Non-ASCII characters](#umlaut-非asc码字符-non-ascii-characters)
- [Apostrophe 撇号](#apostrophe-撇号)
- [排序 ORDER BY 与逆序 DESC](#排序-order-by-与逆序-desc)
- [取前 K 个数用 LIMIT](#取前-k-个数用-limit)
- [CASE 与 ORDER BY 混用](#case-与-order-by-混用)
- [嵌套查询、去重 DISTINCT、聚合 GROUP BY](#嵌套查询-去重-distinct-聚合-group-by)
- [混合练习：CONCAT与处理NULL值](#混合练习concat与处理null值)
- [correlated sub-query or synchronized sub-query（子查询同步？）](#correlated-sub-query-or-synchronized-sub-query子查询同步)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [基础的查询与运算](#基础的查询与运算)
  - [热身：WHERE, IN, BETWEEN, LIKE](#热身where-in-between-like)
  - [基本运算：AND, OR, 加减乘除, length](#基本运算and-or-加减乘除-length)
  - [异或XOR, 圆整ROUND, LEFT与不等号NOT](#异或xor-圆整round-left与不等号not)
- [Umlaut 非ASC码字符 Non-ASCII characters](#umlaut-非asc码字符-non-ascii-characters)
- [Apostrophe 撇号](#apostrophe-撇号)
- [排序 ORDER BY 与逆序 DESC](#排序-order-by-与逆序-desc)
- [取前 K 个数用 LIMIT](#取前-k-个数用-limit)
- [CASE 与 ORDER BY 混用](#case-与-order-by-混用)
- [嵌套查询、去重 DISTINCT、聚合 GROUP BY](#嵌套查询-去重-distinct-聚合-group-by)
  - [简单嵌套查询及其用处（条件筛选/运算/ALL等）](#简单嵌套查询及其用处条件筛选运算all等)
  - [嵌套与DISTINCT用法](#嵌套与distinct用法)
  - [GROUPBY](#groupby)
- [混合练习：CONCAT与处理NULL值](#混合练习concat与处理null值)
  - [字符串合并CONCAT](#字符串合并concat)
  - [可用大于0处理NULL](#可用大于0处理null)
- [correlated sub-query or synchronized sub-query（子查询同步？）](#correlated-sub-query-or-synchronized-sub-query子查询同步)

<!-- /code_chunk_output -->


### 基础的查询与运算

#### 热身：WHERE, IN, BETWEEN, LIKE

```sql
SELECT population FROM world
  WHERE name = 'Germany'

SELECT name, population FROM world
  WHERE name IN ('Sweden', 'Norway', 'Denmark');

SELECT name, population
  FROM world
 WHERE continent = 'South America'

SELECT name, area FROM world
  WHERE area BETWEEN 200000 AND 250000

SELECT name, population
FROM world
WHERE name LIKE "Al%"

--Show the countries which have a name that includes the word 'United'
SELECT name
FROM world
WHERE name LIKE '%united%'
```

#### 基本运算：AND, OR, 加减乘除, length

```sql
--Select the code which shows the countries that end in A or L
SELECT name FROM world
 WHERE name LIKE '%a' OR name LIKE '%l'

SELECT name,length(name)
FROM world
WHERE length(name)=5 and region='Europe'

--Show the name and capital where the name and the capital
--  have the same number of characters.
SELECT name, capital
  FROM world
 WHERE LENGTH(name)=LENGTH(capital)

SELECT name, area*2 FROM world WHERE population = 64000

--the countries with an area larger than 50000 and a population smaller than 10000000
SELECT name, area, population
  FROM world
 WHERE area > 50000 AND population < 10000000

--the population density of China, Australia, Nigeria and France
SELECT name, population/area
  FROM world
 WHERE name IN ('China', 'Nigeria', 'France', 'Australia')

--Give the name and the per capita GDP for those countries with a population of at least 200 million.
SELECT name, gdp / population
  FROM world
 WHERE population >= 200000000
```

#### 异或XOR, 圆整ROUND, LEFT与不等号NOT

异或`XOR`：

```sql
/*
Exclusive OR (XOR).
  Show the countries that are big by area (more than 3 million)
  or big by population (more than 250 million) but not both.
  Show name, population and area.
*/
SELECT name, population, area
  FROM world
 WHERE area > 3000000 XOR population > 250000000

--等价于上面的
SELECT name, population, area
  FROM world
 WHERE (area > 3000000 AND population < 250000000)
  OR (area < 3000000 and population > 250000000)
```

圆整`ROUND`：

```sql
SELECT name, ROUND(population/1000000,2), ROUND(gdp/1000000000, 2)
  FROM world
 WHERE continent = 'South America'

--取到 1000 ，用负数
SELECT name, ROUND(gdp/population, -3)
  FROM world
 WHERE gdp > 1000000000000
```

不等 `<>` 与 `LEFT`：

```sql
/*
不等： <>
取左值： LEFT
The capital of Sweden is Stockholm. Both words start with the letter 'S'.

Show the name and the capital where the first letters of each match.
  Don't include countries where the name and the capital are the same word.
*/
SELECT name,  capital
  FROM world
 WHERE LEFT(name,1) = LEFT(capital,1) and name<>capital
```

不包含的例子：
```sql
/*
Equatorial Guinea and Dominican Republic have all of the vowels (a e i o u) in the name.
  They don't count because they have more than one word in the name.

Find the country that has all the vowels and no spaces in its name.

You can use the phrase name NOT LIKE '%a%' to exclude characters from your results.
The query shown misses countries like Bahamas and Belarus because they contain at least one 'a'
*/
SELECT name FROM world
  WHERE name LIKE '%a%' AND 
 name  LIKE '%e%' AND 
 name  LIKE '%i%' AND 
 name  LIKE '%o%' AND 
 name  LIKE '%u%' AND 
 name NOT LIKE '% %'  --不含空格

/*
Show the year, subject,
  and name of winners for 1980
  excluding Chemistry and Medicine
*/
SELECT *
  FROM nobel
 WHERE yr = 1980 AND subject NOT IN ('Chemistry', 'Medicine')
```

### Umlaut 非ASC码字符 Non-ASCII characters

Find all details of the prize won by PETER GRÜNBERG.

Non-ASCII characters:
- The u in his name has an umlaut. You may find this link useful
- [https://en.wikipedia.org/wiki/%C3%9C#Keyboarding](https://en.wikipedia.org/wiki/%C3%9C#Keyboarding).

```sql
SELECT *
  FROM nobel
 WHERE winner LIKE 'peter gr%nberg'
```

|yr|subject|winner|
|---|---|---|
|2007|Physics|Peter Grünberg|

### Apostrophe 撇号

Find all details of the prize won by `EUGENE O'NEILL`.

两个 `''` 相当于字符 `'` 。

```sql
SELECT *
FROM nobel
WHERE winner = 'Eugene O''Neill'
```

### 排序 ORDER BY 与逆序 DESC

List the winners, year and subject where the winner starts with Sir. Show the the most recent first, then by name order.

```sql
SELECT winner, yr, subject
FROM nobel
WHERE winner LIKE 'sir%'
ORDER BY yr DESC, winner
```

### 取前 K 个数用 LIMIT

```sql
SELECT name, population
  FROM bbc
  WHERE population > 100000000
  ORDER BY population DESC LIMIT 3
```

如上，取最大的前 3 个数（先降序，再 `LIMIT`）。

### CASE 与 ORDER BY 混用

Show the 1984 winners and subject ordered by subject and winner name; but list Chemistry and Physics last.

查找1984年获奖者和主题，按主题和获胜者名称排序，并把化学奖和物理奖排到最后面显示。

```sql
SELECT winner, subject
FROM nobel
WHERE yr=1984
ORDER BY
CASE
WHEN subject IN ('Physics','Chemistry')
THEN 1
ELSE 0
END ASC,
subject,
winner
```

啥意思呢，这句 `CASE WHEN subject IN ('Physics','Chemistry') THEN 1 ELSE 0 END` 会让属于'物理'和'化学'的返回 `1` ，否则返回 `0` ，然后按照 `ASC` 升序排列，因此属于 `1` 的就到后面去，接着再按照 `subject` 和 `winner` 排序。

其实这里 `subject IN ('Physics','Chemistry') ` 自动返回 `1` 和 `0` 因此可以用不 `case` ：

```sql
SELECT winner, subject
FROM nobel
WHERE yr=1984
ORDER BY subject IN ('Physics','Chemistry') ASC,
subject,
winner
```

### 嵌套查询、去重 DISTINCT、聚合 GROUP BY

#### 简单嵌套查询及其用处（条件筛选/运算/ALL等）

条件筛选：
```sql
--List each country in the same continent as 'Brazil'.
SELECT name FROM world WHERE continent =
  (SELECT continent FROM world WHERE name='Brazil')
/*
  有些版本的 sql 可以用 AS 设置表的别名
*/
SELECT name FROM world WHERE continent = 
  (SELECT continent FROM world WHERE name='Brazil') AS brazil_continent

--List each country and its continent in the same continent as 'Brazil' or 'Mexico'.
SELECT name, continent FROM world
WHERE continent IN
  (SELECT continent 
     FROM world WHERE name='Brazil'
                   OR name='Mexico')

-- List the name and continent of countries in the continents
--  containing either Argentina or Australia.
--  Order by name of the country.
SELECT name, continent
FROM world
WHERE continent IN
  (SELECT continent FROM world WHERE name IN ('Argentina', 'Australia'))
ORDER BY name
```

运算：
```sql
--Show the population of China as a multiple of
--  the population of the United Kingdom
SELECT
 population/(SELECT population FROM world
             WHERE name='United Kingdom')
  FROM world
WHERE name = 'China'

/*
Result:
population/(S..
21.2987
*/

--Show the countries in Europe with a per capita GDP
--  greater than 'United Kingdom'.
SELECT name
FROM world
WHERE gdp/population >
    (SELECT gdp/population FROM world WHERE name = 'United Kingdom') AND continent = 'Europe'
```

`ALL` 和 `ANY` 可以同运算联用：

You can use the words `ALL` or `ANY `where the right side of the operator might have multiple values.

```sql
--Show each country that has a population greater than
--  the population of ALL countries in Europe.

--Note that we mean greater than every single country in Europe;
--  not the combined population of Europe.
SELECT name FROM world
 WHERE population > ALL
      (SELECT population FROM world
        WHERE continent='Europe')
```

#### 嵌套与DISTINCT用法

```sql
--Select the code which would show the year
--  when neither a Physics or Chemistry award was given
SELECT yr FROM nobel
 WHERE yr NOT IN(SELECT yr 
                   FROM nobel
                 WHERE subject IN ('Chemistry','Physics'))

--Pick the code that shows the amount of
--  years where no Medicine awards were given
SELECT COUNT(DISTINCT yr) FROM nobel
 WHERE yr NOT IN (SELECT DISTINCT yr FROM nobel WHERE subject = 'Medicine')

--Select the code which shows the years
--  when a Medicine award was given but no Peace or Literature award was
SELECT DISTINCT yr
  FROM nobel
 WHERE subject='Medicine' 
   AND yr NOT IN(SELECT yr FROM nobel 
                  WHERE subject='Literature')
   AND yr NOT IN (SELECT yr FROM nobel
                   WHERE subject='Peace')
```

#### GROUPBY

`GROUP BY` 也很直观：

原表：
```
nobel
yr	subject	winner
1960	Chemistry	Willard F. Libby
1960	Literature	Saint-John Perse
1960	Medicine	Sir Frank Macfarlane Burnet
1960	Medicine	Peter Medawar
1960	Physics	Donald A. Glaser
1960	Peace	Albert Lutuli
```

```sql
 SELECT subject, COUNT(subject) 
   FROM nobel 
  WHERE yr ='1960' 
  GROUP BY subject
```

查询后：
```
Chemistry	1
Literature	1
Medicine	2
Peace	1
Physics	1
```

### 混合练习：CONCAT与处理NULL值

#### 字符串合并CONCAT

```sql
/*
Germany (population 80 million) has the largest population of the
  countries in Europe. Austria (population 8.5 million) has 11% of the
  population of Germany.

Show the name and the population of each country in Europe. Show the
  population as a percentage of the population of Germany.

The format should be Name, Percentage for example:

name	percentage
Albania	3%
Andorra	0%
Austria	11%
...	...
Decimal places
Percent symbol %
You can use the function CONCAT to add the percentage symbol.
*/
SELECT name, CONCAT(
  ROUND(
    100 * population / (
      SELECT population FROM world WHERE name = 'Germany'), 0), '%')
FROM world
WHERE continent = 'Europe'
```

#### 可用大于0处理NULL

处理 `NULL` ：
```sql
SELECT name
FROM world
WHERE gdp >= ALL(
  SELECT gdp FROM world WHERE gdp >=0 AND continent = 'Europe') AND continent != 'Europe'
```

### correlated sub-query or synchronized sub-query（子查询同步？）

A correlated subquery works like a nested loop: the subquery only has access to rows related to a single record at a time in the outer query. The technique relies on table aliases to identify two different uses of the same table, one in the outer query and the other in the subquery.

One way to interpret the line in the WHERE clause that references the two table is “… where the correlated values are the same”.

```sql
--Find the largest country (by area) in each continent,
--  show the continent, the name and the area:
SELECT continent, name, area FROM world x
  WHERE area >= ALL
    (SELECT area FROM world y
        WHERE y.continent = x.continent
          AND area > 0)

--巧妙地应用了字母序比较
--List each continent and the name of the country
--  that comes first alphabetically.
SELECT continent, name
FROM world x
WHERE name <= ALL(SELECT name FROM world y WHERE y.continent = x.continent)

--Find the continents where all countries have a population <= 25000000.
--  Then find the names of the countries associated with these continents.
--  Show name, continent and population.
SELECT name, continent, population
FROM world x
WHERE 25000000 > ALL(  /* 把这个洲筛选出来 */
  SELECT population
  FROM world y
  WHERE x.continent = y.continent AND y.population > 0)

--Some countries have populations more than three times that
--  of any of their neighbours (in the same continent).
--  Give the countries and continents.
SELECT name, continent
FROM world x
WHERE population > ALL(  /* 把这个洲其他国家筛选出来 */
  SELECT population * 3 FROM world y
  WHERE x.continent = y.continent
  AND population > 0 AND y.name != x.name)
```
