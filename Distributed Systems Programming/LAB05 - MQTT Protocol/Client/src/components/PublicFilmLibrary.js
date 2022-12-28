import React from 'react';
import { Table, Button } from 'react-bootstrap/'
import { Link, useLocation } from 'react-router-dom';
import Pagination from "react-js-pagination";

function PublicFilmTable(props) {

  const handlePageChange = pageNumber => {
    props.refreshFilms(pageNumber);
  }

  

  return (
    <>
    <Table>
      <tbody>
        {
          props.films.map((film) =>
            <PublicFilmRow filmData={film} key={film.id} id={film.id}
              deleteFilm={props.deleteFilm} updateFilm={props.updateFilm} />
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

function PublicFilmRow(props) {

  /* location hook is used to pass state to the edit view (or add view). 
   * So, we may be able to come back to the last selected filter view if cancel is pressed.
   */
  const location = useLocation();

  return (
    <tr>
      <td>
       {
        props.filmData.owner == localStorage.getItem("userId") &&
        <Link to={"/public/edit/" + props.filmData.id} state={{nextpage: location.pathname}}>
          <i className="bi bi-pencil-square" />
        </Link>
      }
      &nbsp; &nbsp;
      {
        props.filmData.owner == localStorage.getItem("userId") &&
        <Link to={{}}> 
          <i className="bi bi-trash" onClick={() => { props.deleteFilm(props.filmData) }} />
        </Link>
      }
      </td>
      <td>
        <p className={ [ 'keep-white-space', props.filmData.favorite ? "bi-favorite" : "" ].join(' ')}>
          {`${props.filmData.title}`}
        </p>
      </td>
      <td>
        <Link to={"/public/" + props.filmData.id + "/reviews"} state={{nextpage: location.pathname}}>
          <Button variant="primary">Read Reviews</Button>{' '}
        </Link>
      </td>
      <td>
       {
        props.filmData.owner == localStorage.getItem("userId") &&
        <Link to={"/public/" + props.filmData.id + "/issue"} state={{nextpage: location.pathname}}>
          <Button variant="secondary">Issue Review</Button>{' '}
        </Link>
      }
      </td>
    </tr>
  );
}

export default PublicFilmTable;