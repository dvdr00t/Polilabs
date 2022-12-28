import React from 'react';
import { Table, Button, OverlayTrigger, Popover } from 'react-bootstrap/'
import { Link, useLocation } from 'react-router-dom';
import Pagination from "react-js-pagination";

function FilmReviewTable(props) {

  const handlePageChange = pageNumber => {
    props.refreshReviews(parseInt(props.film.id), pageNumber);
  }

  return (
    <>
    <Table>
      <tbody>
        {
          props.reviews.map((review) =>
            <FilmReviewRow reviewData={review} filmData={props.film} key={review.reviewerId} id={review.reviewerId}
              deleteReview={props.deleteReview} updateReview={props.updateReview} />
          )
        }
        
      </tbody>
    </Table>

       <Pagination 
          itemClass="page-item" // add it for bootstrap 4
          linkClass="page-link" // add it for bootstrap 4
          activePage={parseInt(localStorage.getItem("currentPage"))}
          itemsCountPerPage={parseInt(localStorage.getItem("totalItems"))/parseInt(localStorage.getItem("totalPages"))}
          totalItemsCount={parseInt(localStorage.getItem("totalItems"))}
          pageRangeDisplayed={10}
          onChange={handlePageChange}
          pageSize ={parseInt(localStorage.getItem("totalPages"))}
      />
    </>

  );
}

function FilmReviewRow(props) {

  const formatWatchDate = (dayJsDate, format) => {
    return dayJsDate.isValid() ? dayJsDate.format(format) : '';
  }

  /* location hook is used to pass state to the edit view (or add view). 
   * So, we may be able to come back to the last selected filter view if cancel is pressed.
   */
  const location = useLocation();

  
  return (
    <tr>
      <td>
       {
        props.reviewData.reviewerId == localStorage.getItem("userId") &&
        <Link to={"/public/" + props.reviewData.filmId + "/reviews/complete"} state={{nextpage: location.pathname}}>
          <i className="bi bi-pencil-square" />
        </Link>
      }
      &nbsp; &nbsp;
      {
        props.filmData.owner == localStorage.getItem("userId") &&
        <Link to={{}}> 
          <i className="bi bi-trash" onClick={() => { props.deleteReview(props.reviewData) }} />
        </Link>
      }
      </td>
      <td>
        <p>Reviewer ID: {props.reviewData.reviewerId}</p>
      </td>
      <td>
      {
        !props.reviewData.completed &&
        <p>Not Completed</p>
      }
      {
        props.reviewData.completed &&
        <p>Completed</p>
      }
      </td>
      <td>
        {props.reviewData.reviewDate ? <small>{formatWatchDate(props.reviewData.reviewDate, 'MMMM D, YYYY')}</small> : ''}
      </td>
      <td>
        {props.reviewData.rating ? <Rating rating={props.reviewData.rating} maxStars={10} /> : ''}   
      </td>
      <td>
        {props.reviewData.review ? 
        <OverlayTrigger
        trigger="click" placement="left"
        overlay={
          <Popover>
            <Popover.Header as="h3">Review</Popover.Header>
            <Popover.Body>
              {props.reviewData.review}
            </Popover.Body>
          </Popover>
        }
      >
        <Button variant="secondary">Review Text</Button>
      </OverlayTrigger>
        : ''}
      </td>
    </tr>
  );
}

function Rating(props) {
  return [...Array(props.maxStars)].map((el, idx) =>
    <i  key={idx} className={(idx < props.rating) ? "bi bi-star-fill" : "bi bi-star"} onClick={() => props.updateRating(idx+1)}/>
  )
}
export default FilmReviewTable;