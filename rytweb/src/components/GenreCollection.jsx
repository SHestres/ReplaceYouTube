import {useState, useEffect} from 'react'
import VideoCard from './VideoCard';

export default function GenreCollection({ genre })
{
    const [content, setContent] = useState([]);

    useEffect(() => {
        console.log("Fetching genre")
        fetch(`http://localhost:5100/api/list/${genre.name}`)
        .then((res) => res.json())
        .then(data => setContent(data))
        .catch(e => console.log("Error fetching movies in genre"))
    }, [genre.name])

    return(
    <div className="movie-row">
        <div className="movie-category-label">{genre.name}</div>
        <div className="row">
            <div className="translate-row">
                {content.map((movie) => <VideoCard key={movie.id} {...movie} />)}
            </div>
        </div>
    </div>
    )
}
