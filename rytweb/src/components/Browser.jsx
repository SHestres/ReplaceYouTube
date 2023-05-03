import GenreCollection from './GenreCollection';
import Layout from './Layout'
import { useContext, useState, useEffect} from 'react'
import { useParams } from 'react-router-dom/';
import useGlobalState from '../services/useGlobalState';

export default function Browser(){
    const [genres, setGenres] = useState([]);
    const [isLoading, setIsLoading] = useState(true);
    const [hasGenre, setHasGenre] = useState(false);
    const {category} = useParams();

    //Allows things to be run when a sensitivity list changes
    useEffect(()=>{
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/videos/data/genres.json`)
        .then((res) => res.json())
        .then((res) => {
            setGenres(res);
            setIsLoading(false)
        })
        .catch(() => console.log("Error getting genres"));
    }, []) //Empyt arr as second param means it will run once when Browser() is run (loaded)

    const [cats, setCats] = useState();

    useEffect(()=>{
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/list/categories`)
        .then((res) => res.json())
        .then((res) => {
            console.log(res);
            setCats(res);
            setIsLoading(false);
        })
        .catch(() => console.log("Error getting categories"));
    }, [])

    useEffect(() =>{
        hasGenreCb(false);
    }, [category])

    const hasGenreCb = function(val){
        setHasGenre(val);
    }

    return(
        !isLoading && (
            <Layout>
            {genres.map((genre) => 
                <GenreCollection key={genre.id} genre={genre} category={category} hasGenreCallback={hasGenreCb}/>
            )}
            {console.log(hasGenre)}
            {
                hasGenre ? null: (
                    <>
                        <h2 className='movie-category-label'>{cats[category]}</h2>
                        <div className="no-show-text">Add videos for them to show up here!</div>
                    </>
                )
            }
            </Layout>
        )
    )
}