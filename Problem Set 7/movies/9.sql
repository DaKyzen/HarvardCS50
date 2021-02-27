-- Takes 1395ms
-- SELECT people.name
-- FROM people
-- WHERE people.id IN
-- 	(SELECT stars.person_id
-- 	FROM stars
-- 	WHERE stars.movie_id IN
-- 		(SELECT movies.id
-- 		FROM movies
-- 		WHERE movies.year = '2004'
-- 		)
-- 	)
-- ORDER BY people.birth

-- Takes 1092
SELECT people.name
FROM people
WHERE people.id IN
	(SELECT stars.person_id
	FROM stars
	INNER JOIN movies
	ON movies.id = stars.movie_id
	WHERE movies.year = '2004')
ORDER BY people.birth

