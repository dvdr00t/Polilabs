import React from 'react';
import { ListGroup } from 'react-bootstrap/';
import { NavLink } from 'react-router-dom';

/**
 * 
 * This components requires:
 * - the list of filters labels to show, 
 * - the filter that is currenctly selected 
 * - the handler to notify a new selection
 */
const RouteFilters = (props) => {
  const { items, selected } = props;

  return (
    <ListGroup as="ul" variant="flush">
      {
        items.map((item) => {
          return (
            <NavLink key={item} to={`/${item}`} style={{ textDecoration: 'none' }}>
              <ListGroup.Item as="li" key={item} 
                action active={selected === item} >
                {item.charAt(0).toUpperCase() + item.slice(1).replace('/', ' ').replace('_', ' ')}
              </ListGroup.Item>
            </NavLink>
          );
        })
      }
    </ListGroup>
  )
}

export { RouteFilters };

