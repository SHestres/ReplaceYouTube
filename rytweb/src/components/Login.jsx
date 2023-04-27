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

    function keyPressHandler(evt){
        if(evt.keyCode === 13)
        {
            doLogin();
        }
    }

    //Sign in button callback
    function doLogin(){
        console.log("fetching");
        let content = JSON.stringify({username, password});
        fetch('http://localhost:5100/api/login', {
            method: 'POST',
            body: content,
            headers: {'Content-Length': content.length, 'Content-Type': 'application/json'}
        })
        .then((res) => res.json())
        .then((res)=> {
            //console.log(res);
            setUsername('');
            setPassword('');
            //Clear the vars in global state (and form fields)
            if(res.user) {
                //Store the username into the global state
                dispatch({type: 'login', user: res.user });
                //console.log(res.user);
                //Redirect the use to '/'
                history.push('/');
            }
        })
        .catch((e) => console.log('There was an error logging in'));
    }

    return(
        <Layout>
            <div className="login-panel">
                <h1>Sign In</h1>
                <input type='text' placeholder='Username' onChange={setUsernameHandler} value={username} onKeyDown={keyPressHandler}></input>
                <input type='text' placeholder='Password' onChange={setPasswordHandler} value={password} onKeyDown={keyPressHandler}></input>
                <button id='signInButton' onClick={doLogin}>Sign In</button>
            </div>
            <img className="login-bg" src={bgImg} alt="Login"></img>
        </Layout>
    )
}