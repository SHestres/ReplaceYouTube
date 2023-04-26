import Layout from './Layout'
import bgImg from '../assets/sign-in-background.jpg'
import {useState, useContext} from 'react'
import useGlobalState from '../services/useGlobalState'
import { useHistory} from 'react-router-dom'

export default function Login()
{
    //useState returns a state variable and a function to update it. Used to hook into the global state
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');
    const { StateContext } = useGlobalState();
    const { dispatch } = useContext(StateContext);
    const history = useHistory();

    //Inputs need a callback for accepting input
    function setUsernameHandler(evt) {
        setUsername(evt.target.value);
    }

    function setPasswordHandler(evt){
        setPassword(evt.target.value);
    }

    //Sign in button callback
    function doLogin(){
        fetch('/api/login', {
            method: 'POST',
            body: JSON.stringify({username, password}),
            headers: {'Content-Type': 'application/json'}
        }).then(res => res.json()).then(res=> {
            //Clear the vars in global state (and form fields)
            if(res.user) {
                setUsername('');
                setPassword('');
                //Store the username into the global state
                dispatch({type: 'login', user: res.user });
                //Redirect the use to '/'
                history.push('/');
            }
        })
    }

    return(
        <Layout>
            <div className="login-panel">
                <h1>Sign In</h1>
                <input type='text' placeholder='Username' onChange={setUsernameHandler} value={username}></input>
                <input type='text' placeholder='Password' onChange={setPasswordHandler} value={password}></input>
                <button id='signInButton' onClick={doLogin}>Sign In</button>
            </div>
            <img className="login-bg" src={bgImg} alt="Login"></img>
        </Layout>
    )
}