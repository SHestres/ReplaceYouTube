import { useState, useContext, useEffect} from "react";
import UserProfileButton from "./UserProfileButton";
import { uniqueId } from 'lodash';
import '../profiles.css'

export default function ProfileSelect(){

    const [users, setUsers] = useState();
    const [isLoading, setIsLoading] = useState(true);

    useEffect(() =>{
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/profile/list`)
        .then((res) => res.json())
        .then((res)=> {
            setUsers(res);
            setIsLoading(false);
        })
        .catch((e) => console.log('There was an error logging in'));
    }, []);

    

    return (
        !isLoading &&
        <div className='profile-select'>
            <div className='profile-row'>
                {console.log(users)}
                {users.map((usr)=>
                    <UserProfileButton key={uniqueId()} userName={usr}/>
                )}
            </div>
        </div>
    )
}