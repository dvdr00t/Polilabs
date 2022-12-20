import React, { useState } from 'react';
import Card from './Cards'

const OnlineList = ({usersList}) => {


    const cardComponent = usersList.map(user => {
      
      let selectedInfo=0;
      
      for(var i=0;i<usersList.length;i++){
        if(usersList[i].userId==user.userId){
          selectedInfo = usersList[i]
        }
      }
     

      return <Card key={user.userId}  selectedInfo={selectedInfo} id={user.userId} name={user.userName}/>
    })
    return(
        <div>
        {cardComponent}
        </div>
    );

}

export default OnlineList;
