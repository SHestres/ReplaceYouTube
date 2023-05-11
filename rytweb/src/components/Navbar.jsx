import {Link} from 'react-router-dom'
import { useState, useEffect } from 'react';

export default function Navbar()
{
    const [cats, setCats] = useState();
    const [isLoading, setIsLoading] = useState(true);

    useEffect(()=>{
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/list/categories`)
        .then((res) => res.json())
        .then((res) => {
            setCats(res);
            setIsLoading(false);
        })
        .catch(() => console.log("Error getting categories"));
    }, [])

    return(
        !isLoading &&
        <div className="navbar">
            <Link className='navbar-btn' to="/">Home</Link>
            {Object.keys(cats).map((cat) => 
                (<Link key={cat} className='navbar-btn' to={`/browse/${cat}`}>{cats[cat]}</Link>)
            )}
        </div>
    )
}