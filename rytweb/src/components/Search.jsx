import Layout from "./Layout";
import { useState, useEffect } from "react";
import VideoCard from "./VideoCard";
import { uniqueId } from "lodash";
import SearchGrid from "./SearchGrid";


export default function Search(){

    const [cats, setCats] = useState({});
    const [input, setInput] = useState("");
    const [isFinished, setIsFinished] = useState(false);

    useEffect(()=>{
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/list/categories`)
        .then((res) => res.json())
        .then((res) => {
            setCats(res);
            setIsFinished(true);
        })
        .catch(() => console.log("Error getting categories"));
    }, []);

    const updateInput = (evt) =>{
        setInput(evt.target.value)
    }

    return(
        isFinished && 
        <Layout>
            <div className="search-wrapper">
                <input className="search-bar" placeholder="Search..." onChange={updateInput}/>
            </div>
            <div className="search-row" >
                {Object.keys(cats).map( (cat) =>
                    <SearchGrid key={uniqueId()} input={input} cat={cat} />
                )}
            </div>
        </Layout>
    )
}