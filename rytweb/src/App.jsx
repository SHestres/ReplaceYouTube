import {Switch, Route} from 'react-router-dom'
import Login from './components/Login'
import Browser from './components/Browser'
import useGlobalState from './services/useGlobalState'

import './App.css'

function App() {
  const {StateProvider} = useGlobalState();
  return (
    <StateProvider>
      <div className="App">
        <Switch>
          <Route path="/" exact>
            <h1>Homepage</h1>
          </Route>
          <Route path="/login">
            <Login/>
          </Route>
          <Route path="/browse">
            <Browser/>
          </Route>
        </Switch>
      </div>
    </StateProvider>
  )
}

export default App