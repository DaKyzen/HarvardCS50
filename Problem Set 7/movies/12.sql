-- SELECT movies.title, stars.person_id
-- FROM movies
-- 	INNER JOIN stars
-- 	ON stars.movie_id = movies.id
-- WHERE stars.person_id =
-- 	(SELECT people.id
-- 	FROM people
-- 	WHERE people.name = 'Helena Bonham Carter' OR people.name = 'Johnny Depp'
-- 	)

-- SELECT movies.title, stars.person_id
-- FROM movies
-- 	INNER JOIN stars
-- 	ON stars.movie_id = movies.id
-- WHERE stars.person_id =
-- 		(SELECT people.id
-- 		FROM people
-- 		WHERE people.name = 'Helena Bonham Carter')
-- 	OR stars.person_id = 
-- 		(SELECT people.id
-- 		FROM people
-- 		WHERE people.name = 'Johnny Depp')
SELECT movies.title
FROM movies
	INNER JOIN stars
	ON stars.movie_id = movies.id
WHERE stars.person_id =
	(SELECT people.id
	FROM people
	WHERE people.name = 'Johnny Depp')
INTERSECT
SELECT movies.title
FROM movies
	INNER JOIN stars
	ON stars.movie_id = movies.id
WHERE stars.person_id =
	(SELECT people.id
	FROM people
	WHERE people.name = 'Helena Bonham Carter')