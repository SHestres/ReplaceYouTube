import useGlobalState from "../services/useGlobalState";
import { useContext } from "react";
import { Redirect } from "react-router-dom"


export default function DetectAuth()
{
    const {StateContext} = useGlobalState();
    const {state} = useContext(StateContext);

    return(
        state.user ? <Redirect to='/browse' /> : <Redirect to='/login' />
    )
}
