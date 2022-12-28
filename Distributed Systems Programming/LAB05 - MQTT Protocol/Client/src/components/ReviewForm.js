import dayjs from 'dayjs';
import React, {useState} from 'react';
import { Form, Button } from 'react-bootstrap';
import { Link, useNavigate, useLocation } from 'react-router-dom';
import { Review } from '../models/Review';



const ReviewForm = (props) => {

  const [filmId, setFilmId] = useState(props.review ? props.review.filmId : '');
  const [reviewerId, setReviewerId] = useState(props.review ? props.review.reviewerId : '');
  const [completed, setCompleted] = useState(props.review ? props.review.completed : false);
  const [reviewDate, setReviewDate] = useState(props.review ? (props.review.reviewDate ? props.review.reviewDate : "") : dayjs().format('YYYY-MM-DD'));
  const [rating, setRating] = useState(props.review ? props.review.rating : 0);
  const [review, setReview] = useState(props.review ? props.review.review : 0);

  // useNavigate hook is necessary to change page
  const navigate = useNavigate();
  // useLocation hook is used to remember with filter was selected when we reached this form
  const location = useLocation();

  const nextpage = location.state?.nextpage || '/';

  const handleSubmit = (event) => {
    event.preventDefault();
    // String.trim() method is used for removing leading and ending whitespaces from the title.
  //const owner = props.film ? props.film.owner : localStorage.getItem("userId");
    const newReview = new Review( {filmId, reviewerId, completed, reviewDate, rating, reviewDate, review } );
    props.editReview(newReview);
    navigate('/');
}


  return (
    <Form className="block-example border border-primary rounded mb-0 form-padding" onSubmit={handleSubmit}>
      <Form.Group className="mb-3">
        <Form.Label>Film ID</Form.Label>
        <Form.Control type="text" required={true} value={filmId} disabled />
      </Form.Group>

      <Form.Group className="mb-3">
        <Form.Label>Reviewer ID</Form.Label>
        <Form.Control type="text" required={true} value={reviewerId} disabled /> 
      </Form.Group>

      <Form.Group className="mb-3">
        <Form.Check custom="true" type="checkbox" label="Completed" name="completed" checked={completed} onChange={(event) => setCompleted(event.target.checked)} />
      </Form.Group>

      <Form.Group className="mb-3">
        <Form.Label>Review Date Date</Form.Label>
        { 
        /* reviewDate is an optional parameter. It have to be properly rendered only if available. */ 
        }
        <Form.Control type="date" value={reviewDate} max={dayjs().format("YYYY-MM-DD")} onChange={(event) => setReviewDate(event.target.value)}/>
      </Form.Group>

      <Form.Group className="mb-3">
        <Form.Label>Rating</Form.Label>
        <Form.Select aria-label="Rating" defaultValue={rating} onChange={event => setRating(event.target.value)}>
          { [...Array(11)].map( (v, i) => <option key={i} value={i}>{i}</option>) }
        </Form.Select>
      </Form.Group>

      <Form.Group className="mb-3">
        <Form.Label>Review Text</Form.Label>
        <Form.Control as="textarea" rows={3} onChange={event => setReview(event.target.value)}/>
      </Form.Group>

      <Button className="mb-3" variant="primary" type="submit">Save</Button>
      &nbsp;
      <Link to={nextpage}> 
        <Button className="mb-3" variant="danger" >Cancel</Button>
      </Link>
    </Form>
  )

}

export default ReviewForm;
