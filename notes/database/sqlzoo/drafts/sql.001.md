# SELECT basics


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [基础的查询与运算](#基础的查询与运算)

<!-- /code_chunk_output -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [基础的查询与运算](#基础的查询与运算)
  - [热身：WHERE, IN, BETWEEN, LIKE](#热身where-in-between-like)
  - [基本运算：AND, OR, 加减乘除, length](#基本运算and-or-加减乘除-length)
  - [异或XOR, 圆整ROUND, LEFT与不等号](#异或xor-圆整round-left与不等号)

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

#### 异或XOR, 圆整ROUND, LEFT与不等号

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
 name NOT LIKE '% %'
```
