SELECT movies.title AS 'Title', ratings.rating AS 'Rating'
FROM movies
JOIN ratings
ON movies.id = ratings.movie_id
WHERE movies.year = 2010
ORDER BY movies.title