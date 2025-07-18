import {useState, useEffect} from 'react'
import VideoCard from './VideoCard';
import { uniqueId } from 'lodash';
import FavIcon from './FavIcon';

export default function GenreCollection({ genre, category, title, hasGenreCallback })
{
    const [content, setContent] = useState([]);

    useEffect(() => {
        //console.log(`Fetching genre ${genre.name} from category ${category}`)
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/list/${category}/${genre.name}`)
        .then((res) => res.json())
        .then((data) => {
            setContent(data);
            if(data.length != 0) hasGenreCallback(true);
        })
        .catch(e => console.log("Error fetching movies in genre"))
    }, [category, genre.name]);

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
        : false
    )
}
