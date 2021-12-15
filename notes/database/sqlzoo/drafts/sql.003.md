# JOIN


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [JOIN basics](#join-basics)

<!-- /code_chunk_output -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [JOIN basics](#join-basics)
  - [基础的 JOIN 用法](#基础的-join-用法)
  - [一些组合应用](#一些组合应用)

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

