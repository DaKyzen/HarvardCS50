SELECT people.name
FROM people
WHERE people.id IN
	(SELECT directors.person_id
	FROM directors
		INNER JOIN movies
		ON movies.id = directors.movie_id
		INNER JOIN ratings
		ON ratings.movie_id = movies.id
		WHERE ratings.rating >= '9.0'
	)
	