import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { TodosComponent } from './todos.component';
import { NO_ERRORS_SCHEMA } from '@angular/core';
import { ApiMockService } from '../api-mock.service';
import { TodoDataService } from '../todo-data-service.service';
import { ApiService } from '../api.service';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs/Observable';

describe('TodosComponent', () => {
  let component: TodosComponent;
  let fixture: ComponentFixture<TodosComponent>;

  beforeEach(async(() => {
  TestBed.configureTestingModule({
    declarations: [TodosComponent],
    schemas: [
      NO_ERRORS_SCHEMA
    ],
    //Provider for testbed options
    providers: [
      TodoDataService,
      {
        provide: ApiService,
        useClass: ApiMockService
      },
      {
        provide: ActivatedRoute,
        useValue: {
          data: Observable.of({
            todos: []
          })
        }
      }
    ],
  })
    .compileComponents();
}));

  beforeEach(() => {
    fixture = TestBed.createComponent(TodosComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
