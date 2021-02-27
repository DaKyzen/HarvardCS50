-- SELECT movies.title
-- FROM movies
-- WHERE movies.id IN
-- 	(SELECT ratings.movie_id
-- 	FROM ratings
-- 		INNER JOIN movies
-- 		ON ratings.movie_id = movies.id
-- 	WHERE ratings.movie_id IN
-- 		(SELECT stars.movie_id
-- 		FROM stars
-- 			INNER JOIN people
-- 			ON stars.person_id = people.id
-- 			WHERE people.name = 'Chadwick Boseman'
-- 		)
-- 	ORDER BY ratings.rating DESC
-- 	LIMIT 5
-- 	)

	
-- SELECT movies.title
-- FROM movies
-- 	INNER JOIN ratings
-- 	ON movies.id = ratings.movie_id
-- WHERE movies.id IN
-- 	(SELECT stars.movie_id
-- 	FROM stars
-- 		INNER JOIN people
-- 		ON stars.person_id = people.id
-- 		WHERE people.name = 'Chadwick Boseman'
-- 	)
-- ORDER BY ratings.rating DESC
-- LIMIT 5

SELECT movies.title
FROM movies
	INNER JOIN ratings
	ON movies.id = ratings.movie_id
WHERE movies.id IN
	(SELECT stars.movie_id
	FROM stars
	WHERE stars.person_id =
		(SELECT people.id
		FROM people
		WHERE people.name = 'Chadwick Boseman'
		)
	)
ORDER BY ratings.rating DESC
LIMIT 5