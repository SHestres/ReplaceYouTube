import Logo from '../assets/youtube-logo-blue.svg'
import {FaRegCopyright} from 'react-icons/fa'
import Navbar from './Navbar'
import Profile from './Profile'


export default function Layout({children})
{
    return(<>
        <header>
            <div className="App-header">
                <img id="Logo" src={Logo} width={150} height={150} alt='Welcome to BlueTube'/>
                <Navbar/>
                <Profile/>
            </div>
        </header>
        <section>
            {children}
        </section>
        <footer>
            <FaRegCopyright/> No rights reserved. Have fun.
        </footer>
    </>
    )
}