/*
Router Configuration
*/
import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { AppComponent } from './app.component';
import { TodosComponent } from './todos/todos.component';
import { PageNotFoundComponent } from './page-not-found/page-not-found.component';
import { TodosResolver } from './todos.resolver';

//Variable definition of type Routes
//Order is important due to reachable states
const routes: Routes = [
  {
    path: '',    //Empty
    redirectTo: 'todos',
    pathMatch: 'full'
  },
  {
    path: 'todos',  //todos
    component: TodosComponent,
    resolve: {                  //Resolver
      todos: TodosResolver
    }
  },
  {
    path: '**',   //Wildcard path to PageNotFound
    component: PageNotFoundComponent
  }
];

//Create and export an Angular module
@NgModule({
  //RouterModule.forChild(routes) needed for multiple routing modules
  imports: [RouterModule.forRoot(routes)],
  //Since we export it, Angular will import Routing Module automatically when
  //importing AppRoutingModule, no need to explicity import RouterModule again
  exports: [RouterModule], 
  providers: [
    TodosResolver      //Register for DI
  ] 
})
export class AppRoutingModule {
}