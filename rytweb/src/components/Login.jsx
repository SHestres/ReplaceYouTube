import Layout from './Layout'
import bgImg from '../assets/sign-in-background.jpg'

export default function Login()
{
    return(
        <Layout>
            <div className="login-panel">
                <h1>Sign In</h1>
                <input type='text' placeholder='Username'></input>
                <input type='text' placeholder='Password'></input>
                <button id='signInButton'>Sign In</button>
            </div>
            <img className="login-bg" src={bgImg}></img>
        </Layout>
    )
}