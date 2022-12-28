import React from 'react';
import ListGroup from 'react-bootstrap/ListGroup';

const MiniOnlineList = (props) => {
    const {onlineList} = props;

    const createUserItem = (user) => {
        return (
            <ListGroup.Item key={user.userId}><img src={require("../world.png").default} alt={"Eagle"} width="20" height="20"  />{ " User: "+user.userName}</ListGroup.Item>
        );
    }
        
    
    return (
        <>
           <ListGroup  variant="flush">
                <ListGroup.Item key={0} className="p-3 mt-5 list-title">Online Users</ListGroup.Item>
                {onlineList.map(user => createUserItem(user)) }
            </ListGroup>
        </>
    );
}



export default MiniOnlineList;
