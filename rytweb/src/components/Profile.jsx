import useGlobalState from "../services/useGlobalState"
import { useContext } from "react"

export default function Profile()
{
    
    const {StateContext} = useGlobalState();
    const {state, dispatch} = useContext(StateContext);

    function doLogout()
    {
        dispatch({
            type: 'logout'
        })
    }

    return(
        <div className="profile">
            {state.user ? (
                <>
                    {state.user}
                    <button className="logout-btn" onClick={doLogout}>Logout</button>
                </>
            ) : null}
        </div>
    )

}