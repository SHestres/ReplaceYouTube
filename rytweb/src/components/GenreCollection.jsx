import {useState, useEffect} from 'react'
import VideoCard from './VideoCard';
import { uniqueId } from 'lodash';

export default function GenreCollection({ genre, category, title })
{
    const [content, setContent] = useState([]);

    useEffect(() => {
        //console.log(`Fetching genre ${genre.name} from category ${category}`)
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/list/${category}/${genre.name}`)
        .then((res) => res.json())
        .then(data => setContent(data))
        .catch(e => console.log("Error fetching movies in genre"))
    }, [genre.name]);

    return(
        content.length > 0 ? 
        (
            <div className="movie-row">
                <div className="movie-category-label">{title? title : genre.name}</div>
                <div className="row">
                    <div className="translate-row">
                        {content.map((movie) => <VideoCard key={uniqueId()} {...movie} />)}
                    </div>
                </div>
            </div>
        ) 
        : category == "favorites" ?  
            <div className="movie-row">
                <div className="movie-category-label">{title}</div>
                <div className="row">
                    <div className="no-show-text">Mark favorites for them to show up here!</div>
                </div>
            </div>
        : null
    )
}
