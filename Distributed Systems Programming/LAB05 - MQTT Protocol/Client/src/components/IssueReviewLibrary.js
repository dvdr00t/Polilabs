import React, {useEffect} from 'react';
import { Dropdown, Button} from 'react-bootstrap/'
import Select from 'react-select/'
import _ from 'lodash'

function IssueReviewTable(props) {

  let userId="-1";

  useEffect(() => {
    props.getUsers();
}, []);

  const usersOptions = _.map(props.users, (id, index) => ({
    value: props.users[index].userId,
    label: props.users[index].userName,
  }))

  const handleUsersDropdown = (e) => {
    userId = e.value;
  }

  function assignUsers() {
    var chosenUser = null;
    for(const user of props.users){
      if(user.userId == userId){
        chosenUser = user;
      }
  }
    if(chosenUser != null){
      props.issueReview(props.filmId, chosenUser);
    }
  }



  return (
    <div>
       <p></p>
       <p>Select the user:</p>
       <Select options={usersOptions} onChange={handleUsersDropdown}/>
       <p></p>
       <Button onClick={assignUsers} variant="outline-primary" size="lg" className="fixed-right">Issue Review</Button>
    </div>
    
 
  );
}

export default IssueReviewTable;

//<Dropdown placeholder='Users' fluid clearable selection options={usersOptions} onChange={handleUsersDropdown} />