import {useReducer, createContext} from 'react'

const initialState = {
    user: null,
    rendering: false
}

const GlobalStateContext = createContext();

//When dispatch is used, only the action parameter is used
function reducer(state, action) {
    switch(action.type) {
        case 'login':{
            return{...state, user: action.user }
        }
        case 'logout':{
            return{...state, user: null}
        }
        case 'checkRender':{
            return{...state, rendering: action.rendering}
        }
        default:
            throw new Error('Invalid reducer action');
    }
}

//Used to acces the global state (established in this file) throughout the project
export default function useGlobalState(){
    const [state, dispatch] = useReducer(reducer, initialState);

    return {
        StateProvider: function({children}) {
            return <GlobalStateContext.Provider value={{state, dispatch}}>
                {children}
            </GlobalStateContext.Provider>
        },
        StateContext : GlobalStateContext
    }
}