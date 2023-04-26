import {Switch, Route} from 'react-router-dom'
import Login from './components/Login'
import './App.css'

function App() {
  return (
    <div className="App">
      <Switch>
        <Route path="/" exact>
          <h1>Homepage</h1>
        </Route>
        <Route path="/login">
          <Login/>
        </Route>
        <Route path="/browse">
          <h1>Browing page</h1>
        </Route>
      </Switch>
    </div>
  )
}

export default App