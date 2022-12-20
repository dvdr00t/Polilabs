import React from 'react';
import { Card } from "semantic-ui-react";



const Cards = (props) => {
   const {id, name, selectedInfo} = props;
    return (
      <Card>
      <Card.Content header={"User Name: " +name} />
      <Card.Content description={"UsedID: " + id}/>
      <Card.Content extra>
      {selectedInfo.filmId ? "Film Selected: "+selectedInfo.filmId+" "+ selectedInfo.filmTitle : 'Film not selected'}
      </Card.Content>
    </Card>
    );
}

const styleLink = document.createElement("link");
styleLink.rel = "stylesheet";
styleLink.href = "https://cdn.jsdelivr.net/npm/semantic-ui/dist/semantic.min.css";
document.head.appendChild(styleLink);

export default Cards;