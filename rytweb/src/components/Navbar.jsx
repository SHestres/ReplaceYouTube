import {Link} from 'react-router-dom'

export default function Navbar()
{
    return(
        <nav>
            <Link className='navbar-btn' to="/">Home</Link>
            <Link className='navbar-btn' to="/browse/movies">Movies</Link>
        </nav>
    )
}