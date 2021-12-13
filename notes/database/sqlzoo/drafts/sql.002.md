# FUNCTIONS


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [Using SUM, Count, MAX, DISTINCT and ORDER BY](#using-sum-count-max-distinct-and-order-by)
- [Using GROUP BY and HAVING](#using-group-by-and-having)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Using SUM, Count, MAX, DISTINCT and ORDER BY](#using-sum-count-max-distinct-and-order-by)
- [Using GROUP BY and HAVING](#using-group-by-and-having)

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
