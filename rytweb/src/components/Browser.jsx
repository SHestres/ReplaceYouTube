import GenreCollection from './GenreCollection';
import Layout from './Layout'
import {useState, useEffect} from 'react'

export default function Browser(){
    const [genres, setGenres] = useState([]);
    const [isLoading, setIsLoading] = useState(true);

    //Allows things to be run when a sensitivity list changes
    useEffect(()=>{
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/genres`)
        .then((res) => res.json())
        .then((res) => {
            setGenres(res);
            setIsLoading(false)
        })
        .catch(() => console.log("Error getting genres"));
    }, []) //Empyt arr as second param means it will run once when Browser() is run (loaded)

    return(
        !isLoading && (
            <Layout>
            {genres.map((genre) => 
                <GenreCollection key={genre.id} genre={genre} />
            )}
            </Layout>
        )
    )
}