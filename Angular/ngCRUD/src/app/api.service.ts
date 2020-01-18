import { Injectable } from '@angular/core';
import { environment } from 'environments/environment';

import { Http, Response } from '@angular/http';
import { Todo } from './todo';

//Pulling just what we need from RxJS, operators are attached to Observable
import { Observable } from 'rxjs/Observable';
import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';
import 'rxjs/add/observable/throw';

const API_URL = environment.apiUrl;

@Injectable()
export class ApiService {

  constructor(
    private http: Http
  ) {
  }

  // API: GET /todos
  public getAllTodos(): Observable<Todo[]> {
  return this.http
    .get(API_URL + '/todos')			//GET all 'todos' returns Observable
    .map(response => {					//map: Transform response from Api into an array of Todo objects
      const todos = response.json();	//Response is a string, response.json() to parse into JS values
      return todos.map((todo) => new Todo(todo));	//Returns array of 'todos'
    })
    .catch(this.handleError);
}

  // API: POST /todos
  public createTodo(todo: Todo): Observable<Todo> {
  return this.http
    .post(API_URL + '/todos', todo)		//POST(URL, data)
    .map(response => {					//Transform repsonse into Todo object
      return new Todo(response.json());
    })
    .catch(this.handleError);
}

  // API: GET /todos/:id
  public getTodoById(todoId: number): Observable<Todo> {
  return this.http
    .get(API_URL + '/todos/' + todoId)
    .map(response => {
      return new Todo(response.json());
    })
    .catch(this.handleError);
}

  // API: PUT /todos/:id
  public updateTodo(todo: Todo): Observable<Todo> {
  return this.http
    .put(API_URL + '/todos/' + todo.id, todo)	//PUT(URL, data)
    .map(response => {
      return new Todo(response.json());
    })
    .catch(this.handleError);
}

  // DELETE /todos/:id
  public deleteTodoById(todoId: number): Observable<null> {
  return this.http
    .delete(API_URL + '/todos/' + todoId)	//DELETE(URL, id)
    //.map(response => null) not needed shows possiblity of manipulating data
    .catch(this.handleError);
}

  //Error Handler
  private handleError (error: Response | any) {
  	console.error('ApiService::handleError', error);
  return Observable.throw(error);
  }
}