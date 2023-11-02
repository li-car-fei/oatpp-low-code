
CREATE TABLE AppUser (
    id          INTEGER PRIMARY KEY,
    username    VARCHAR UNIQUE,
    email       VARCHAR UNIQUE,
    password    VARCHAR,
    role        VARCHAR
);

INSERT INTO AppUser
(username, email, password, role) VALUES ('admin', 'admin@domain.com', 'admin', 'ROLE_ADMIN');


CREATE TABLE Project (
    project_id          INTEGER PRIMARY KEY,
    user_id             INTEGER,
    project_title       VARCHAR,
    project_description VARCHAR,
    FOREIGN KEY (user_id) REFERENCES AppUser(id)
);

INSERT INTO Project
(user_id, project_title, project_description) VALUES (1, 'project 1 title here', 'project 1 description here');


CREATE TABLE ComponentRecord (
    record_id           INTEGER PRIMARY KEY,
    project_id          INTEGER,
    component_type      VARCHAR NOT NULL,
    components_index    INTEGER,
    currentDate         VARCHAR,
    header              VARCHAR,
    title               VARCHAR,
    content             VARCHAR,
    mode                VARCHAR,
    src                 VARCHAR,
    link                VARCHAR,
    label1              VARCHAR,
    label2              VARCHAR,
    backgroundColor     VARCHAR,
    FOREIGN KEY (project_id) REFERENCES Project(project_id)
);

INSERT INTO ComponentRecord
(project_id, component_type, components_index, backgroundColor) VALUES (1, 'whiteSpace', 1, '#ffffff');
INSERT INTO ComponentRecord
(project_id, component_type, components_index, content) VALUES (1, 'richText', 2, '富文本编辑器');
INSERT INTO ComponentRecord
(project_id, component_type, components_index, label1, label2) VALUES (1, 'picker', 3, 'black', 'white');
INSERT INTO ComponentRecord
(project_id, component_type, components_index, header) VALUES (1, 'list', 4, '列表标题');


CREATE TABLE ChildComponentRecord (
    record_id               INTEGER PRIMARY KEY,
    component_id            INTEGER,
    child_components_index   INTEGER,
    description_            VARCHAR,
    title                   VARCHAR,
    clickable               INTEGER,
    src                     VARCHAR,
    link                    VARCHAR,
    text_                   VARCHAR,
    content                 VARCHAR,
    FOREIGN KEY (component_id) REFERENCES ComponentRecord(record_id)
);

INSERT INTO ChildComponentRecord
(component_id, child_components_index, content, description_, title, clickable) VALUES (4, 1, '内容一', '描述一', '标题一', 1);
INSERT INTO ChildComponentRecord
(component_id, child_components_index, content, description_, title, clickable) VALUES (4, 2, '内容二', '描述二', '标题二', 0);
INSERT INTO ChildComponentRecord
(component_id, child_components_index, content, description_, title, clickable) VALUES (4, 3, '内容三', '描述三', '标题三', 1);


