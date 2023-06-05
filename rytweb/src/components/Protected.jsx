import useGlobalState from '../services/useGlobalState'
import { useContext } from 'react'
import {Redirect, Route} from 'react-router-dom'

//Anything inside the protected tag will only appear when there is a logged in user
export default function Protected({children, redirect, ...rest }){
    const {StateContext } = useGlobalState();
    const {state} = useContext(StateContext);

    return (state.user ? <Route user={state.user} {...rest}>{children}</Route> : (redirect ? <Redirect to={redirect} /> : <Redirect to='/login' />))
}