import { useState } from 'react';
import { Form, Button, Alert, Col, Row } from 'react-bootstrap';
import {useLocation, useNavigate} from 'react-router-dom';


function LoginForm(props) {
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');

  const [show, setShow] = useState(false);
  const [errorMessage, setErrorMessage] = useState('');

  const location = useLocation();
  const navigate = useNavigate();

  // if user has been redirected here from another page, go back to that urls
  const oldPath = location?.state?.pathname || "";


  const handleSubmit = (event) => {
    event.preventDefault();
    const credentials = { email, password };

    props.login(credentials)
      .then( () => navigate( oldPath ) )
      .catch((err) => { 
        setErrorMessage(err.error); setShow(true); 
      });
  };

  return (
    <Row className="vh-100 justify-content-md-center">
    <Col md={4} >
    <h1 className="pb-3">Login</h1>

      <Form  onSubmit={handleSubmit}>
          <Alert
            dismissible
            show={show}
            onClose={() => setShow(false)}
            variant="danger">
            {errorMessage}
          </Alert>
          <Form.Group className="mb-3" controlId="email">
            <Form.Label>email</Form.Label>
            <Form.Control
              type="email"
              value={email} placeholder="Example: john.doe@polito.it"
              onChange={(ev) => setEmail(ev.target.value)}
              required={true}
            />
          </Form.Group>
          <Form.Group className="mb-3" controlId="password">
            <Form.Label>Password</Form.Label>
            <Form.Control
              type="password"
              value={password} placeholder="Enter the password."
              onChange={(ev) => setPassword(ev.target.value)}
              required={true} minLength={6}
            />
          </Form.Group>
          <Button className="mt-3" type="submit">Login</Button>
      </Form>
      </Col>
      </Row>

  )
};

function LogoutButton(props) {
  return (
        <Button variant="outline-light" onClick={props.logout}>Logout</Button>
  )
}

export { LoginForm, LogoutButton };
