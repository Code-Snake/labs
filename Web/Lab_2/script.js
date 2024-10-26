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
