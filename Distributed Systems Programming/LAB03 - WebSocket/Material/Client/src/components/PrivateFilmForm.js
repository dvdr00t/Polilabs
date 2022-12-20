import dayjs from 'dayjs';
import React, {useState} from 'react';
import { Form, Button } from 'react-bootstrap';
import { Link, useNavigate, useLocation } from 'react-router-dom';
import { Film } from '../models/Film';



const PrivateFilmForm = (props) => {

  /*
   * Creating a state for each parameter of the film.
   * There are two possible cases: 
   * - if we are creating a new film, the form is initialized with the default values.
   * - if we are editing a film, the form is pre-filled with the previous values.
   */
  const [title, setTitle] = useState(props.film ? props.film.title : '');
  const [favorite, setFavorite] = useState(props.film ? props.film.favorite : false);
  // It is preferable to avoid format conversions in the controlled form field, thus we use the date string format used in the DATA form control (YYYY-MM-DD) and convert to dayjs only after validation
  const [watchDate, setWatchDate] = useState(props.film ? (props.film.hasDate() ? props.film.watchDate.format('YYYY-MM-DD') : "") : dayjs().format('YYYY-MM-DD'));
  const [rating, setRating] = useState(props.film ? props.film.rating : 0);
  const [privateFilm, setPrivateFilm] = useState(props.film ? props.film.private : true);

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
  const film = new Film( {"title": title.trim(), "owner": parseInt(owner), privateFilm, favorite, rating, watchDate  } );

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
          <option key={true} value={true}>True</option>
        </Form.Select>
      </Form.Group>

      <Form.Group className="mb-3">
        <Form.Check custom="true" type="checkbox" label="Favorite" name="favorite" checked={favorite} onChange={(event) => setFavorite(event.target.checked)} />
      </Form.Group>

      <Form.Group className="mb-3">
        <Form.Label>Watch Date</Form.Label>
        { 
        /* watchDate is an optional parameter. It have to be properly rendered only if available. */ 
        }
        <Form.Control type="date" value={watchDate} max={dayjs().format("YYYY-MM-DD")} onChange={(event) => setWatchDate(event.target.value)}/>
      </Form.Group>

      <Form.Group className="mb-3">
        <Form.Label>Rating</Form.Label>
        <Form.Select aria-label="Rating" defaultValue={rating} onChange={event => setRating(event.target.value)}>
          { [...Array(11)].map( (v, i) => <option key={i} value={i}>{i}</option>) }
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

export default PrivateFilmForm;
