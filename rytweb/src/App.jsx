import {Switch, Route} from 'react-router-dom'
import Login from './components/Login'
import Browser from './components/Browser'
import useGlobalState from './services/useGlobalState'
import Protected from './components/Protected'
import FetchVideo from './components/FetchVideo'
import Player from './components/Player'

import './App.css'
import DetectAuth from './components/DetectAuth'

function App() {
  const {StateProvider} = useGlobalState();
  return (
    <StateProvider>
      <div className="App">
        <Switch>
          <Route path="/" exact>
            <DetectAuth />
          </Route>
          <Route path="/login">
            <Login/>
          </Route>
          <Protected path="/browse">
            <Browser/>
          </Protected>
          <Route path="/play/:id">
            <FetchVideo>
              {video => <Player {...video} />}
            </FetchVideo>
          </Route>
        </Switch>
      </div>
    </StateProvider>
  )
}

export default App