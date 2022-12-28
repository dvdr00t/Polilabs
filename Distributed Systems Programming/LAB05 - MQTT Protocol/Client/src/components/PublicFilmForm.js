import dayjs from 'dayjs';
import React, {useState} from 'react';
import { Form, Button } from 'react-bootstrap';
import { Link, useNavigate, useLocation } from 'react-router-dom';
import { Film } from '../models/Film';



const PublicFilmForm = (props) => {

  /*
   * Creating a state for each parameter of the film.
   * There are two possible cases: 
   * - if we are creating a new film, the form is initialized with the default values.
   * - if we are editing a film, the form is pre-filled with the previous values.
   */
  const [title, setTitle] = useState(props.film ? props.film.title : '');
  const [privateFilm, setPrivateFilm] = useState(props.film ? props.film.private : false);

  // useNavigate hook is necessary to change page
  const navigate = useNavigate();
  // useLocation hook is used to remember with filter was selected when we reached this form
  const location = useLocation();

  // if the film is saved (eventually modified) we return to the list of all films, 
  // otherwise, if cancel is pressed, we go back to the previous location (given by the location state)
  const nextpage = location.state?.nextpage || '/';

  const handleSubmit = (event) => {
    event.preventDefault();


    // String.trim() method is used for removing leading and ending whitespaces from the title.
  const owner = props.film ? props.film.owner : localStorage.getItem("userId");
  const film = new Film( {"title": title.trim(), "owner": parseInt(owner), privateFilm} );

  if(props.film === undefined){
    props.addFilm(film);
  }
  else {
    film.id = props.film.id;
    props.editFilm(film);
  }

    navigate('/');
}


  return (
    <Form className="block-example border border-primary rounded mb-0 form-padding" onSubmit={handleSubmit}>
      <Form.Group className="mb-3">
        <Form.Label>Title</Form.Label>
        <Form.Control type="text" required={true} value={title} onChange={event => setTitle(event.target.value)}/>
      </Form.Group>

      <Form.Group className="mb-3">
        <Form.Label>Private Film</Form.Label>
        <Form.Select aria-label="Private" defaultValue={privateFilm} onChange={event => setPrivateFilm(event.target.value)}>
          <option key={false} value={true}>False</option>
        </Form.Select>
      </Form.Group>

      <Button className="mb-3" variant="primary" type="submit">Save</Button>
      &nbsp;
      <Link to={nextpage}> 
        <Button className="mb-3" variant="danger" >Cancel</Button>
      </Link>
    </Form>
  )

}

export default PublicFilmForm;
