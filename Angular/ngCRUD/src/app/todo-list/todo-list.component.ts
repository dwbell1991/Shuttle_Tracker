/*
	A dumb component, parent of TodoListItemComponent
*/
import { Component, EventEmitter, Input, Output } from '@angular/core';
import { Todo } from '../todo';

@Component({
  selector: 'app-todo-list',
  templateUrl: './todo-list.component.html',
  styleUrls: ['./todo-list.component.css']
})
export class TodoListComponent {

	//input property 'todos', inject 'todos' from parent component
	@Input()
	todos: Todo[];

	//Typescript generic, event emitter instance, values emitted are of Todo instance
	@Output()
	remove: EventEmitter<Todo> = new EventEmitter();

		//Typescript generic, event emitter instance, values emitted are of Todo instance
	@Output()
	toggleComplete: EventEmitter<Todo> = new EventEmitter();

	constructor() {
	}

	/*Event Handlers*/
	onToggleTodoComplete(todo: Todo) {
	  this.toggleComplete.emit(todo);
	}

	onRemoveTodo(todo: Todo) {
	  this.remove.emit(todo);
	}

}