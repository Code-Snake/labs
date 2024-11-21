let currentIndex = 0; // текущий индекс фотографий
const photos = [
    "https://sun9-46.userapi.com/impg/E80s5V3UcDcbyC1SGa_832Wgr-Q_UpOeMhwwRw/PUGe_Zi-xwA.jpg?size=1620x2160&quality=95&sign=5afe2dae7b253fbfb3acc0047b1b168b&type=album",
    "https://sun9-48.userapi.com/impg/WK0erCPG-du_5qCtoAK6VcXeMoLHMkdBrzZrVw/K2gUJvfLuvo.jpg?size=1620x2160&quality=95&sign=dfb7167d9ebc1d59bf7b6fab7bfc21db&type=album", // Добавьте свой URL
    "https://sun9-24.userapi.com/impg/6b9bz1x2AVF4nnpguD1DhIcvL4vL2hwTRPaC-Q/y2TqEKycXbw.jpg?size=1620x2160&quality=95&sign=5eaf0ac059c17da75d391f22e181c02a&type=album",
];

const photoElement = document.querySelector('.my_photo');
const prevButton = document.querySelector('.prev');
const nextButton = document.querySelector('.next');

function updatePhoto() {
    photoElement.src = photos[currentIndex];
}

nextButton.addEventListener('click', () => {
    currentIndex = (currentIndex + 1) % photos.length; // Перейти к следующему индексу
    updatePhoto();
});

prevButton.addEventListener('click', () => {
    currentIndex = (currentIndex - 1 + photos.length) % photos.length; // Перейти к предыдущему индексу
    updatePhoto();
});

setInterval(() => {
    currentIndex = (currentIndex + 1) % photos.length; // Перейти к следующему индексу
    updatePhoto();
}, 5000); // Смена изображений каждые 5 секунд
const commentList = document.getElementById('comment-list');
const commentInput = document.getElementById('comment-input');
const addCommentButton = document.getElementById('add-comment');

// Функция для добавления комментария в DOM
function addComment(text, save = true) {
    const commentDiv = document.createElement('div');
    commentDiv.className = 'comment';

    const commentText = document.createElement('span');
    commentText.textContent = text;

    const deleteButton = document.createElement('button');
    deleteButton.textContent = 'Удалить';
    deleteButton.addEventListener('click', () => {
        commentDiv.remove();
        if (save) saveComments(); // Обновить сохраненные комментарии
    });

    commentDiv.appendChild(commentText);
    commentDiv.appendChild(deleteButton);
    commentList.appendChild(commentDiv);

    if (save) saveComments(); // Сохраняем комментарий
}

// Событие на добавление комментария
addCommentButton.addEventListener('click', () => {
    const text = commentInput.value.trim();
    if (text) {
        addComment(text);
        commentInput.value = '';
    }
});

// Сохранение комментариев в текстовый файл через сервер
function saveComments() {
    const comments = Array.from(commentList.querySelectorAll('.comment span')).map(
        comment => comment.textContent
    );

    fetch('/save-comments', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ comments }),
    });
}

// Загрузка комментариев с сервера
function loadComments() {
    fetch('/load-comments')
        .then(response => response.json())
        .then(data => {
            data.comments.forEach(comment => addComment(comment, false));
        });
}

// Загрузить комментарии при загрузке страницы
loadComments();
