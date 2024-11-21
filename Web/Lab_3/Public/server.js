const express = require('express');
const fs = require('fs');
const app = express();

app.use(express.json());
app.use(express.static('Public'));// Статичные файлы (HTML, CSS, JS)

const FILE_PATH = 'comments.txt';

// Загрузка комментариев
app.get('/load-comments', (req, res) => {
    fs.readFile(FILE_PATH, 'utf8', (err, data) => {
        if (err) return res.json({ comments: [] });
        res.json({ comments: data.split('\n').filter(line => line) });
    });
});

// Сохранение комментариев
app.post('/save-comments', (req, res) => {
    const comments = req.body.comments.join('\n');
    fs.writeFile(FILE_PATH, comments, err => {
        if (err) return res.status(500).send('Ошибка сохранения');
        res.send('Сохранено');
    });
});

// Запуск сервера
app.listen(3000, () => console.log('Сервер запущен на http://localhost:3000'));
