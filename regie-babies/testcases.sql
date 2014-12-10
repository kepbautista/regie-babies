SELECT * FROM table_name;
SELECT studno,birthday FROM student;
select studcourse.semester,
student.studno, student.studentname
FROM STUDENT
JOIN studcourse
ON student.studno=studentcourse.studno;
-- WHERE version
select studcourse.semester,
student.studno, student.studentname
FROM STUDENT, STUDCOURSE
where student.studno=studcourse.studno;
SELECT * FROM COurse where Semoffered=1st;
INSERT INTO STUDENT VALUES ('2008-00196',"Kristine
Bautista","1991-05-15","BS",'CS',44);
INSERT INTO COURSE VALUES ('CMSC 124','Design and Implementation of
Programming Languages','PL stuff',3,1,'1st');
UPDATE course SET Ctitle='CMSC 127';
INSERT INTO Courseoffering VALUES ('1st','2010-2011','CMSC
124','T-2L','16:00',20);
DELETE FROM course WHERE ctitle='CMSC 124';
insert into studcourse (studno,cno,semester,acadyear) VALUES (1,2,1st,1);
update course set cno="database",haslab=1*2,noofunits=2*(noofunits+(noofunits*1)) where cno="cmsc 127";