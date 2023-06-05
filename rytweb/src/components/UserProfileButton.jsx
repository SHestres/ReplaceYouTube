import useGlobalState from "../services/useGlobalState";
import { useState, useContext, useEffect} from "react";
import { useHistory } from 'react-router-dom'
import { BsEmojiSmile } from "react-icons/bs";

export default function ({userName}) {

    const { StateContext } = useGlobalState();
    const { dispatch } = useContext(StateContext);
    const history = useHistory();

    const setUser = () =>{
        //Store the username into the global state
        dispatch({type: 'login', user:userName });
        //Redirect the use to '/'
        history.push('/');
    }

    return <button className='profile-select-button' onClick={setUser}>
        <div><BsEmojiSmile /></div>
        {userName}
    </button>
}