import { Injectable } from '@angular/core';
import { ActivatedRouteSnapshot, Resolve, RouterStateSnapshot } from '@angular/router';
import { Observable } from 'rxjs/Observable';
import { Todo } from './todo';
import { TodoDataService } from './todo-data-service.service';

//Defining the resolver as a class that implements Resolve
@Injectable()
export class TodosResolver implements Resolve<Observable<Todo[]>> {

  constructor(
    private todoDataService: TodoDataService
  ) {
  }

  //This method is called when routers needs to resolve data
  //Angular passes in activated route snapshot and router state snapshot
  //so we have access to useful information (route parameters etc)
  public resolve(
    route: ActivatedRouteSnapshot,
    state: RouterStateSnapshot
    //Returns a value, promise or observable. 
  ): Observable<Todo[]> {
    return this.todoDataService.getAllTodos();
  }
}