SELECT AVG(ratings.rating) AS 'Average Movie Ratings of 2012'
FROM ratings
JOIN movies
ON ratings.movie_id = movies.id
WHERE movies.year = 2012