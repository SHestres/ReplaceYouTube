import { useState } from "react"
import {AiFillHeart, AiOutlineHeart} from "react-icons/ai"

export default function FavIcon({className, Favorite, id}){

    const [fav, setFav] = useState(Favorite)

    const toggleFav = function (){
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/fav/${id}?fav=${!fav}`, {method: "POST"})
        .then(setFav(!fav))
        .catch(e => console.log("Error changing fav"))
    }

    return (
        <>
            {fav ? <AiFillHeart className={className} onClick={toggleFav}/> 
            : <AiOutlineHeart className={className} onClick={toggleFav}/>}
        </>
    )
}