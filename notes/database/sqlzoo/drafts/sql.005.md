# ALTER, UNION, LEFT JOIN, NULL


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

### Full Text Search

### Equi Join

### Column Name with Spaces
