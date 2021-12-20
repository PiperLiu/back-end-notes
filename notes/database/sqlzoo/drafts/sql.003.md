# JOIN


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [JOIN basics](#join-basics)
- [INNER JOIN](#inner-join)
- [LEFT JOIN](#left-join)
- [RIGHT JOIN](#right-join)
- [JOIN 与 COALCASE、CASE](#join-与-coalcase-case)
- [SELF JOIN](#self-join)
- [FROM 多个表](#from-多个表)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [JOIN basics](#join-basics)
  - [基础的 JOIN 用法](#基础的-join-用法)
  - [一些组合应用](#一些组合应用)
- [INNER JOIN](#inner-join)
- [LEFT JOIN](#left-join)
- [RIGHT JOIN](#right-join)
- [JOIN 与 COALCASE、CASE](#join-与-coalcase-case)
- [SELF JOIN](#self-join)
- [FROM 多个表](#from-多个表)

<!-- /code_chunk_output -->

### JOIN basics

![](./images/FootballERD.png)

```
game
id	mdate	stadium	team1	team2
1001	8 June 2012	National Stadium, Warsaw	POL	GRE
1002	8 June 2012	Stadion Miejski (Wroclaw)	RUS	CZE
1003	12 June 2012	Stadion Miejski (Wroclaw)	GRE	CZE
1004	12 June 2012	National Stadium, Warsaw	POL	RUS

goal
matchid	teamid	player	gtime
1001	POL	Robert Lewandowski	17
1001	GRE	Dimitris Salpingidis	51
1002	RUS	Alan Dzagoev	15
1002	RUS	Roman Pavlyuchenko	82

eteam
id	teamname	coach
POL	Poland	Franciszek Smuda
RUS	Russia	Dick Advocaat
CZE	Czech Republic	Michal Bilek
GRE	Greece	Fernando Santos
```

#### 基础的 JOIN 用法

一些简单的 JOIN 应用：
```sql
--show the player, teamid, stadium and mdate for every German goal
SELECT player, teamid, stadium, mdate
FROM game
  JOIN goal ON (id=matchid AND teamid='GER')

--Show the team1, team2 and player for every goal scored by a player
--  called Mario player LIKE 'Mario%'
SELECT team1, team2 , player
FROM game
  JOIN goal ON id=matchid
WHERE player LIKE 'Mario%'

--Show player, teamid, coach, gtime for all goals
--  scored in the first 10 minutes gtime<=10
SELECT player, teamid, coach, gtime
  FROM goal
    JOIN eteam on teamid=id
 WHERE gtime<=10

--List the dates of the matches and the name of the team
--  in which 'Fernando Santos' was the team1 coach.
SELECT mdate, teamname
  FROM game
    JOIN eteam ON (team1=eteam.id)  /* 这里 id game 也有，所以要指明 */
 WHERE coach = 'Fernando Santos'
```

#### 一些组合应用

```sql
--The example query shows all goals scored in the Germany-Greece quarterfinal.
--  Instead show the name of all players who scored a goal against Germany.
SELECT DISTINCT(player)
FROM game
  JOIN goal ON matchid = id
WHERE ((team1='GER' OR team2='GER') AND teamid != 'GER')

--Show teamname and the total number of goals scored.
SELECT teamname, COUNT(player)
FROM eteam
  JOIN goal ON id=teamid
GROUP BY teamname

--Show the stadium and the number of goals scored in each stadium.
SELECT stadium, COUNT(player) AS goals
FROM game
  JOIN goal ON (id=matchid)
GROUP BY stadium

--For every match involving 'POL', show the matchid,
--  date and the number of goals scored.
SELECT matchid, mdate, COUNT(player) AS goals
FROM game
  JOIN goal ON (matchid=id AND (team1 = 'POL' OR team2 = 'POL'))
GROUP BY matchid, mdate

--For every match where 'GER' scored, show matchid,
--  match date and the number of goals scored by 'GER'
SELECT id, mdate, COUNT(player)
FROM game
  JOIN goal ON (id=matchid AND (team1 = 'GER' OR team2 = 'GER') AND teamid='GER')
GROUP BY id, mdate
/* 如上 JOIN 完成了 WHERE 的工作 */
```

一个更加综合的应用：
List every match with the goals scored by each team as shown. This will use "`CASE WHEN`" which has not been explained in any previous exercises.

|mdate|team1|score1|team2|score2|
|---|---|---|---|---|
|1 July 2012|ESP|4|ITA|0|
|10 June 2012|ESP|1|ITA|1|
|10 June 2012|IRL|1|CRO|3|

Notice in the query given every goal is listed. If it was a team1 goal then a 1 appears in score1, otherwise there is a 0. You could `SUM` this column to get a count of the goals scored by team1. Sort your result by mdate, matchid, team1 and team2.

```sql
SELECT mdate,
       team1,
       SUM(CASE WHEN teamid = team1 THEN 1 ELSE 0 END) AS score1,
       team2,
       SUM(CASE WHEN teamid = team2 THEN 1 ELSE 0 END) AS score2 FROM
    game LEFT JOIN goal ON (id = matchid)
    GROUP BY mdate,team1,team2
    ORDER BY mdate, matchid, team1, team2
```

另一个与电影有关的例子，嵌套查找与 `join` 联合使用等内容：
```sql
--List the film title and the leading actor for all of the films
--  'Julie Andrews' played in.
SELECT title, name FROM 
  movie JOIN casting ON (movie.id = movieid)
    JOIN actor ON (actor.id = actorid)
    WHERE ord = 1 AND movieid IN (
      SELECT movieid FROM 
        casting JOIN actor ON (actor.id = actorid)
        WHERE name = 'Julie Andrews'
    )

--Obtain a list, in alphabetical order,
--  of actors who've had at least 15 starring roles.
SELECT name FROM
  actor JOIN casting ON (actorid = actor.id)
  WHERE ord = 1  /* 先把 ord=1 都取出来，再 Groupby */
  GROUP BY name
  HAVING COUNT(movieid) >= 15
  ORDER BY name  /* ORDER 指定的表是最终结果，放在 GROUPBY 后 */

--List the films released in the year 1978 ordered by
--  the number of actors in the cast, then by title.
SELECT title, COUNT(*)  FROM
  movie JOIN casting ON (movie.id = movieid)
  WHERE yr = 1978	
  GROUP BY title
  ORDER BY COUNT(*) DESC, title

--List all the people who have worked with 'Art Garfunkel'.
SELECT name FROM
  actor JOIN casting on (actor.id = actorid)
  WHERE movieid IN (
    SELECT movieid FROM 
      casting JOIN actor on (actor.id = actorid)
      WHERE name = 'Art Garfunkel'
  ) AND name <> 'Art Garfunkel'
```

### INNER JOIN

Note the INNER JOIN misses the teachers with no department and the departments with no teacher.

```sql
SELECT teacher.name, dept.name
 FROM teacher INNER JOIN dept
           ON (teacher.dept=dept.id)
```

### LEFT JOIN

Use a different JOIN so that all teachers are listed.

```sql
SELECT teacher.name, dept.name
  FROM teacher LEFT JOIN dept
  ON (teacher.dept=dept.id)
```

### RIGHT JOIN

Use a different JOIN so that all departments are listed.

```sql
SELECT teacher.name, dept.name
  FROM teacher RIGHT JOIN dept
  ON (teacher.dept=dept.id)
```

### JOIN 与 COALCASE、CASE

```sql
/*
Use COALESCE to print the mobile number. Use the number '07986 444 2266'
  if there is no number given. Show teacher name and mobile number or
  '07986 444 2266'
*/
SELECT name,
  COALESCE(mobile, '07986 444 2266')
  FROM teacher

/*
Use the COALESCE function and a LEFT JOIN to print the teacher name
  and department name. Use the string 'None' where there
  is no department.
*/
SELECT teacher.name, COALESCE(dept.name, 'None')
  FROM teacher LEFT JOIN dept
  ON (teacher.dept=dept.id)

/*
Use COUNT and GROUP BY dept.name to show each department and the number of staff.
  Use a RIGHT JOIN to ensure that the Engineering department is listed.
*/
SELECT dept.name, COUNT(teacher.name)
  FROM teacher RIGHT JOIN dept ON (teacher.dept=dept.id)
  GROUP BY dept.name

/*
Use CASE to show the name of each teacher followed by 'Sci' if the
  teacher is in dept 1 or 2 and 'Art' otherwise.
*/
SELECT teacher.name,
  CASE WHEN dept.id = 1 THEN 'Sci'
       WHEN dept.id = 2 THEN 'Sci'
       ELSE 'Art' END
  FROM teacher LEFT JOIN dept ON (teacher.dept=dept.id)

/*
Use CASE to show the name of each teacher followed by 'Sci' if the teacher is
  in dept 1 or 2, show 'Art' if the teacher's dept is 3 and 'None' otherwise.
*/
SELECT teacher.name,
  CASE WHEN dept.id = 1 THEN 'Sci'
       WHEN dept.id = 2 THEN 'Sci'
       WHEN dept.id = 3 THEN 'Art'
       ELSE 'None' END
  FROM teacher LEFT JOIN dept ON (teacher.dept=dept.id)
```

### SELF JOIN

```sql
/* How many stops are in the database. */
SELECT COUNT(DISTINCT id)
  FROM stops

/*
The query shown gives the number of routes that visit either London Road
  (149) or Craiglockhart (53). Run the query and notice the two services that link
  these stops have a count of 2. Add a HAVING clause to restrict the output to these two routes.
*/
SELECT company, num, COUNT(*) AS visits
  FROM route WHERE stop=149 OR stop=53
  GROUP BY company, num
    HAVING visits=2

/*
Execute the self join shown and observe that b.stop gives all
  the places you can get to from Craiglockhart, without changing routes.
*/
SELECT a.company, a.num, a.stop, b.stop
FROM route a JOIN route b ON
  (a.company=b.company AND a.num=b.num)
WHERE a.stop=53

/*
Change the query so that it shows the services
  from Craiglockhart to London Road.
*/
SELECT a.company, a.num, a.stop, b.stop
FROM route a JOIN route b ON
  (a.company=b.company AND a.num=b.num)
WHERE a.stop=53 AND b.stop=149

/*
The query shown is similar to the previous one, however by joining two copies
  of the stops table we can refer to stops by name rather than by number.
  Change the query so that the services between 'Craiglockhart' and 'London
  Road' are shown. If you are tired of these places try
  'Fairmilehead' against 'Tollcross'
*/
SELECT a.company, a.num, stopa.name, stopb.name
FROM route a JOIN route b ON
  (a.company=b.company AND a.num=b.num)
  JOIN stops stopa ON (a.stop=stopa.id)
  JOIN stops stopb ON (b.stop=stopb.id)
WHERE stopa.name='Craiglockhart' and stopb.name='London Road'

/*
Give a list of all the services which connect stops 115 and 137
  ('Haymarket' and 'Leith')
*/
SELECT DISTINCT a.company, a.num
FROM route a JOIN route b ON
  (a.company =b.company AND a.num=b.num)
  JOIN stops stopa ON (a.stop=stopa.id)
  JOIN stops stopb ON (b.stop=stopb.id)
WHERE stopa.name='Haymarket' AND stopb.name='Leith'

/*
Give a list of the services which connect the stops 'Craiglockhart'
  and 'Tollcross'
*/
SELECT DISTINCT a.company, a.num
FROM route a JOIN route b ON
  (a.company =b.company AND a.num=b.num)
  JOIN stops stopa ON (a.stop=stopa.id)
  JOIN stops stopb ON (b.stop=stopb.id)
WHERE stopa.name='Craiglockhart' AND stopb.name='Tollcross'

/*
Give a distinct list of the stops which may be reached from
  'Craiglockhart' by taking one bus, including 'Craiglockhart' itself,
  offered by the LRT company. Include the company and bus no. of
  the relevant services.
*/
SELECT DISTINCT stopb.name, a.company, a.num
FROM route a 
JOIN route b 
ON a.company = b.company AND a.num = b.num
JOIN stops stopa ON a.stop = stopa.id  /* stopb 是其他有连接车站 */
JOIN stops stopb ON b.stop = stopb.id  /* stopa 是 Craiglockhart */
WHERE stopa.name = 'Craiglockhart';

/*
Find the routes involving two buses that can go from Craiglockhart to Lochend.
  Show the bus no. and company for the first bus, the name of the stop for
  the transfer, and the bus no. and company for the second bus.

Hint:
  Self-join twice to find buses that visit Craiglockhart and Lochend,
  then join those on matching stops.
*/
SET SQL_BIG_SELECTS=1;

WITH sel1 AS (SELECT r1.num, r1.company, s2.name transferstop
FROM route r1
JOIN route r2 ON (r1.num=r2.num AND r1.company=r2.company)
JOIN stops s1 ON s1.id=r1.stop
JOIN stops s2 ON s2.id=r2.stop
WHERE s1.name = 'Craiglockhart'), 

sel2 AS (SELECT s1.name transferstop, r2.num, r2.company
FROM route r1
JOIN route r2 ON (r1.num=r2.num AND r1.company=r2.company)
JOIN stops s1 ON s1.id=r1.stop
JOIN stops s2 ON s2.id=r2.stop
WHERE s2.name = 'Lochend')

SELECT sel1.num, sel1.company, sel1.transferstop, sel2.num, sel2.company
  FROM sel1
  JOIN sel2 ON sel1.transferstop=sel2.transferstop
 ORDER BY 1,3,4

/*
Show it is possible to get from Craiglockhart to Haymarket
*/
SELECT DISTINCT a.name, b.name
  FROM stops a JOIN route z ON a.id=z.stop
  JOIN route y ON y.num = z.num
  JOIN stops b ON y.stop=b.id
 WHERE a.name='Craiglockhart' AND b.name ='Haymarket'

/*
Shows the stops that are on route.num '2A' which can be reached with one bus from Haymarket
*/
SELECT S2.id, S2.name, R2.company, R2.num
  FROM stops S1, stops S2, route R1, route R2
 WHERE S1.name='Haymarket' AND S1.id=R1.stop
   AND R1.company=R2.company AND R1.num=R2.num
   AND R2.stop=S2.id AND R2.num='2A'
```

### FROM 多个表

上面的例子中，有：
```sql
SELECT S2.id, S2.name, R2.company, R2.num
  FROM stops S1, stops S2, route R1, route R2
 WHERE S1.name='Haymarket' AND S1.id=R1.stop
   AND R1.company=R2.company AND R1.num=R2.num
   AND R2.stop=S2.id AND R2.num='2A'
```

如上， `FROM` 多个表，实际上是将这些表全连接，比如 `S1` 有 $n$ 行， $FROM S1 S1$ 就会有 $n \times n$ 行。
