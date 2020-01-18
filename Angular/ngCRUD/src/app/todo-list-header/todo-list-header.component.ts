/*
This handles the logic behind the todo-list-header.component.html
Considered a Dumb component because it has no knowledge of the
data service 'todo-data-service.service'
*/
import { Component, Output, EventEmitter } from '@angular/core';
import { Todo } from '../todo';

@Component({
  selector: 'app-todo-list-header',
  templateUrl: './todo-list-header.component.html',
  styleUrls: ['./todo-list-header.component.css']
})
export class TodoListHeaderComponent {

  /*
   Define newTodo property and assign a new Todo() when component class 
   is instantiated.
   This is the same Todo instance specified in the two way binding expression
   of [(ngModel)] in our view. Whenever the input value changes in the view,
   the value in the component instance is updated. And whenever the value in
   the component instance changes, the value in the input element in the view
   is updated. 
  */
  newTodo: Todo = new Todo();

  //Event emmitter custom to this component
  @Output()
  add: EventEmitter<Todo> = new EventEmitter();

  constructor() {
  }

  addTodo() {
    this.add.emit(this.newTodo);
    this.newTodo = new Todo();
  }

}