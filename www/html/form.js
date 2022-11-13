function handleErrors(response) {
    if (!response.ok) {
        throw Error(response.statusText);
    }
    return response;
}

function uploadData() {
    // alert('The selected file will be uploaded');
    const input = document.getElementById("file");
    const formData = new FormData();
    formData.append(file.name, input.files[0]);
    const response = fetch(`/form_upload`, {
        method: 'POST',
        credentials: 'include', // send cookies, even in CORS
        body: formData,
    })
    .then( (response) => {
        if (response.ok) {
            window.location.href = "form_upload_ok.html";
            return;
        }
        return (response.text());
    })
    .then( (response) => {
        console.log(response);
        document.body.innerHTML = response;
        window.location.href.reload();
    })
    .catch ( error => console.log(error))
}
