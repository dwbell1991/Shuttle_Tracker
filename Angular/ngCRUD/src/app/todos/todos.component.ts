/* src/app/todos/todos.component.ts */
import { Component, OnInit } from '@angular/core';
import { TodoDataService } from '../todo-data-service.service';
import { Todo } from '../todo';
import { ActivatedRoute } from '@angular/router';  //Route data


@Component({
  selector: 'app-todos',
  templateUrl: './todos.component.html',
  styleUrls: ['./todos.component.css'],
  providers: [TodoDataService]
})
export class TodosComponent implements OnInit {

  todos: Todo[] = [];

  constructor(
    //DI to get a handle of information
    private todoDataService: TodoDataService,
    private route: ActivatedRoute
  ) {
  }

  public ngOnInit() {
    this.route.data    //Now using route data to avoid 2x api calls
    .map((data) => data['todos'])
    .subscribe(
      (todos) => {
        this.todos = todos;
      })
  }

  onAddTodo(todo) {
    this.todoDataService
      .addTodo(todo)      
      .subscribe(
        (newTodo) => {
          this.todos = this.todos.concat(newTodo);
        }
      );
  }

  onToggleTodoComplete(todo) {
    this.todoDataService
      .toggleTodoComplete(todo)
      .subscribe(
        (updatedTodo) => {
          todo = updatedTodo;
        }
      );
  }

  onRemoveTodo(todo) {
    this.todoDataService
      .deleteTodoById(todo.id)
      .subscribe(
        (_) => {
          this.todos = this.todos.filter((t) => t.id !== todo.id);
        }
      );
  }
}